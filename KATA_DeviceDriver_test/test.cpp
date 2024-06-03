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
};

class DriverTestFixture : public testing::Test {
public:
	MockDevice device;
	DeviceDriver driver{ &device };

	DriverTestFixture() {
		EXPECT_CALL(device, getDelayForReadCmdInMs())
			.WillRepeatedly(Return(1));
	}
};

TEST_F(DriverTestFixture, readCmd_read_5_times_per_cmd) {
	EXPECT_CALL(device, read(_))
		.Times(5);
	driver.read(0x0);
}

TEST_F(DriverTestFixture, readCmd_return_result_only_same) {
	EXPECT_CALL(device, read(_))
		.Times(5)
		.WillRepeatedly(Return(0));

	EXPECT_EQ(driver.read(0x0), 0x0);
}

TEST_F(DriverTestFixture, readCmd_return_exception_when_read_result_not_equal) {
	EXPECT_CALL(device, read(_))
		.Times(5)
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

TEST_F(DriverTestFixture, full_read) {
	
}

TEST_F(DriverTestFixture, write_read_verify) {
}
