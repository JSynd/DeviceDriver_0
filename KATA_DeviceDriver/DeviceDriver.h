#pragma once
#include "FlashMemoryDevice.h"
#include <stdexcept>

class DeviceDriver
{
public:
    DeviceDriver(FlashMemoryDevice *hardware);
    int read(long address);
    void write(long address, int data);

protected:
    FlashMemoryDevice *m_hardware;

private:
    const int MAX_CONFIRM_CNT = 4;
    void checkReliebility(long address, int read_result);
};


class ReadFailException : public std::exception {
public:
    explicit ReadFailException(char const* _Message) : exception(_Message) {
    }
    char const* what() const override {
        return "Read Fail!";
    }
};