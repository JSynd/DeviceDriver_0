#include "DeviceDriver.h"
#include <Windows.h>

DeviceDriver::DeviceDriver(FlashMemoryDevice *hardware) : m_hardware(hardware)
{}

int DeviceDriver::read(long address)
{
    // TODO: implement this method properly
    int read_result = (int)(m_hardware->read(address));;
    for (int try_cnt = 0; try_cnt < READ_RETRY_CNT - 1; try_cnt++) {
        Sleep(200);
        if (read_result != (int)(m_hardware->read(address))) return 0xFFFFFFFF;
    }
    return read_result;
}

void DeviceDriver::write(long address, int data)
{
    // TODO: implement this method
    m_hardware->write(address, (unsigned char)data);
}
