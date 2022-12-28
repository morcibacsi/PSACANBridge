#include "MessageHandler_217.h"
#include "../Structs/CAN_217.h"

void MessageHandler_217::SetData()
{
    _data[0] = _dataBroker->S_217Byte1;
    _data[1] = _dataBroker->S_217Byte2;
    _data[2] = _dataBroker->S_217Byte3;
    _data[3] = _dataBroker->S_217Byte4;
    _data[4] = _dataBroker->S_217Byte5;
    _data[5] = _dataBroker->S_217Byte6;
    _data[6] = _dataBroker->S_217Byte7;
    _data[7] = _dataBroker->S_217Byte8;

     _wasDataSet = true;
}

