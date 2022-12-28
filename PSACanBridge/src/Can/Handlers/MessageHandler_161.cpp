#include "MessageHandler_161.h"
#include "../Structs/CAN_161_2010.h"

void MessageHandler_161::SetData()
{
    CAN_161_2010_Byte5Struct byte5;
    byte5.data.fuel_tank_capacity_in_liters = _config->FUEL_TANK_CAPACITY_IN_LITERS;

    _data[0] = _dataBroker->S_161Byte1;
    _data[1] = 0;
    _data[2] = _dataBroker->S_161Byte3;
    _data[3] = _dataBroker->S_161Byte4;
    _data[4] = byte5.asByte;
    _data[5] = 0;
    _data[6] = _dataBroker->S_161Byte7;
    _data[7] = 0;

     _wasDataSet = true;
}

