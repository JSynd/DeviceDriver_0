#include "DeviceDriver.h"
#include <Windows.h>

DeviceDriver::DeviceDriver(FlashMemoryDevice *hardware) : m_hardware(hardware)
{}

int DeviceDriver::read(long address)
{
    // TODO: implement this method properly
    int read_result = (int)(m_hardware->read(address));
    checkReliebility(address, read_result);
    return read_result;
}

void DeviceDriver::checkReliebility(long address, int read_result)
{
    int delay_in_ms = m_hardware->getDelayForReadCmdInMs();
    for (int confirm_cnt = 0; confirm_cnt < MAX_CONFIRM_CNT; confirm_cnt++) {
        Sleep(delay_in_ms);
        if ((int)(m_hardware->read(address)) != read_result)
            throw ReadFailException("Read reault has changed!");
    }
}

void DeviceDriver::write(long address, int data)
{
    // TODO: implement this method
    checkErased(address);
    m_hardware->write(address, (unsigned char)data);
}

void DeviceDriver::checkErased(long address)
{
    int ret = m_hardware->read(address);
    if (ret != m_hardware->ERASED) throw WriteFailException("Not Erased!");
}

void DeviceDriver::sequentialFullRead()
{
    long address = m_hardware->getStartAddress();
    long endAddress = m_hardware->getEndAddress();
    for (; address < endAddress; address++) {
        read(address);
    }
}

void DeviceDriver::writeRange(long startAddr, long endAddr, int data)
{
    writeRangeWithoutVerify(startAddr, endAddr, data);
    verifyRaange(startAddr, endAddr, data);
}

void DeviceDriver::writeRangeWithoutVerify(long startAddr, long endAddr, int data)
{
    for (long address = startAddr; address <= endAddr; address++) {
        write(address, data);
    }
}

void DeviceDriver::verifyRaange(long startAddr, long endAddr, int data)
{
    for (long address = startAddr; address <= endAddr; address++) {
        int ret = read(address);
        if (ret != data) throw VerificateionFailException("data mismatch");
    }
}
