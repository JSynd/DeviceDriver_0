#include "DeviceDriver.h"
#include <Windows.h>

DeviceDriver::DeviceDriver(FlashMemoryDevice *hardware) : m_hardware(hardware)
{}

int DeviceDriver::read(long address)
{
    // TODO: implement this method properly
    int read_result;
    for (int try_cnt = 0; try_cnt < READ_RETRY_CNT; try_cnt++) {
        read_result = (int)(m_hardware->read(address));
        Sleep(200);
    }
    return read_result;
}

void DeviceDriver::write(long address, int data)
{
    // TODO: implement this method
    m_hardware->write(address, (unsigned char)data);
}
