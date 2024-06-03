#pragma once

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../KATA_DeviceDriver/DeviceDriver.cpp"

using namespace testing;
using namespace std;

class MockDevice : public FlashMemoryDevice {
public:
	MOCK_METHOD(unsigned char, read, (long address), (override));
	MOCK_METHOD(void, write, (long address, unsigned char data), (override));
};

TEST(DeviceDriverTest, test_read_5_times_per_read_cmd) {
	MockDevice device;
	DeviceDriver driver{ &device };

	EXPECT_CALL(device, read(_))
		.Times(5)
		.WillRepeatedly(Return(0));

	driver.read(0x0);
}

