#include "DeviceDriver.h"
#include <Windows.h>

DeviceDriver::DeviceDriver(FlashMemoryDevice *hardware) : m_hardware(hardware)
{}

int DeviceDriver::read(long address)
{
    // TODO: implement this method properly
    int read_result = (int)(m_hardware->read(address));;
    int delay = m_hardware->getDelayForReadCmdInMs();
    for (int confirm_cnt = 0; confirm_cnt < MAX_CONFIRM_CNT; confirm_cnt++) {
        Sleep(delay);
        if ((int)(m_hardware->read(address)) != read_result)
            throw ReadFailException("Read reault has changed!");
    }
    return read_result;
}

void DeviceDriver::write(long address, int data)
{
    // TODO: implement this method
    m_hardware->write(address, (unsigned char)data);
}
