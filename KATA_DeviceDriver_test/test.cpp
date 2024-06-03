#pragma once

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../KATA_DeviceDriver/DeviceDriver.cpp"

using namespace testing;
using namespace std;

class MockDevice : public FlashMemoryDevice {
public:
	MOCK_METHOD(int, getDelayForReadCmdInMs, (), (override));
	MOCK_METHOD(unsigned char, read, (long address), (override));
	MOCK_METHOD(void, write, (long address, unsigned char data), (override));
	
	long getStartAddress() override
	{
		return startAddress;
	}
	long getEndAddress() override
	{
		return endAddress;
	}

private:
	const long startAddress = 0x0;
	const long endAddress = 0x10;
	unsigned char* written[0x100] = {};
};

class DriverTestFixture : public testing::Test {
public:
	MockDevice device;
	DeviceDriver driver{ &device };

	const int READ_CNT_PER_CMD = 5;

	DriverTestFixture() {
		EXPECT_CALL(device, getDelayForReadCmdInMs())
			.WillRepeatedly(Return(1));
	}
};

TEST_F(DriverTestFixture, readCmd_read_5_times_per_cmd) {
	EXPECT_CALL(device, read(_))
		.Times(READ_CNT_PER_CMD);
	driver.read(0x0);
}

TEST_F(DriverTestFixture, readCmd_return_result_only_same) {
	EXPECT_CALL(device, read(_))
		.Times(READ_CNT_PER_CMD)
		.WillRepeatedly(Return(0));

	EXPECT_EQ(driver.read(0x0), 0x0);
}

TEST_F(DriverTestFixture, readCmd_return_exception_when_read_result_not_equal) {
	EXPECT_CALL(device, read(_))
		.Times(READ_CNT_PER_CMD)
		.WillOnce(Return(1))
		.WillOnce(Return(1))
		.WillOnce(Return(1))
		.WillOnce(Return(1))
		.WillRepeatedly(Return(0));

	EXPECT_THROW(driver.read(0x0), ReadFailException);

	EXPECT_CALL(device, read(_))
		.Times(4)
		.WillOnce(Return(1))
		.WillOnce(Return(1))
		.WillOnce(Return(1))
		.WillRepeatedly(Return(0));

	EXPECT_THROW(driver.read(0x0), ReadFailException);

	EXPECT_CALL(device, read(_))
		.Times(3)
		.WillOnce(Return(1))
		.WillOnce(Return(1))
		.WillRepeatedly(Return(0));

	EXPECT_THROW(driver.read(0x0), ReadFailException);

	EXPECT_CALL(device, read(_))
		.Times(2)
		.WillOnce(Return(1))
		.WillRepeatedly(Return(0));

	EXPECT_THROW(driver.read(0x0), ReadFailException);
}

TEST_F(DriverTestFixture, writeCmd_write_in_erased_cell) {
	EXPECT_CALL(device, read(_))
		.Times(1)
		.WillRepeatedly(Return(device.ERASED));
	EXPECT_CALL(device, write(_, _))
		.Times(1);

	EXPECT_NO_THROW(driver.write(0x0, 'A'));
}

TEST_F(DriverTestFixture, writeCmd_write_exception_in_written_cell) {
	EXPECT_CALL(device, read(_))
		.Times(1)
		.WillRepeatedly(Return('A'));
	EXPECT_CALL(device, write(_, _))
		.Times(0);

	EXPECT_THROW(driver.write(0x0, 'A'), WriteFailException);
}

TEST_F(DriverTestFixture, seq_full_read) {
	long endAddr = device.getEndAddress();
	long startAddr = device.getStartAddress();

	EXPECT_CALL(device, read(_))
		.Times(READ_CNT_PER_CMD * (endAddr - startAddr));

	driver.sequentialFullRead();
}

TEST_F(DriverTestFixture, write_read_verify) {
	long startAddress = 0x0;
	long endAddress = 0x4;
	const int CMD_CNT = endAddress - startAddress + 1;
	const char DATA = 'F';

	EXPECT_CALL(device, write(_, _))
		.Times(CMD_CNT);

	EXPECT_CALL(device, read(_))
		.Times(CMD_CNT)
		.WillRepeatedly(Return(device.ERASED));

	driver.writeRangeWithoutVerify(startAddress, endAddress, DATA);

	EXPECT_CALL(device, read(_))
		.Times(READ_CNT_PER_CMD * CMD_CNT)
		.WillRepeatedly(Return(DATA));

	driver.verifyRaange(startAddress, endAddress, DATA);
	//driver.writeRange(startAddress, endAddress, DATA);
}