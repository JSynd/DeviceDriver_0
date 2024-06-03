#pragma once
#include "FlashMemoryDevice.h"
#include <stdexcept>

class DeviceDriver
{
public:
    DeviceDriver(FlashMemoryDevice *hardware);
    int read(long address);
    void write(long address, int data);

    void sequentialFullRead();
    void writeRange(long startAddr, long endAddr, int data);

    void verifyRaange(long startAddr, long endAddr, int data);

    void writeRangeWithoutVerify(long startAddr, long endAddr, int data);

protected:
    FlashMemoryDevice *m_hardware;

private:
    const int MAX_CONFIRM_CNT = 4;
    void checkReliebility(long address, int read_result);
    void checkErased(long address);
};


class ReadFailException : public std::exception {
public:
    explicit ReadFailException(char const* _Message) : exception(_Message) {
    }
    char const* what() const override {
        return "Read Fail!";
    }
};


class WriteFailException : public std::exception {
public:
    explicit WriteFailException(char const* _Message) : exception(_Message) {
    }
    char const* what() const override {
        return "Write Fail!";
    }
};


class VerificateionFailException : public std::exception {
public:
    explicit VerificateionFailException(char const* _Message) : exception(_Message) {
    }
    char const* what() const override {
        return "Data mismatch!";
    }
};