#include "MessageHandler_0B6.h"

void MessageHandler_0B6::SetData()
{
    _data[0] = _dataBroker->S_0B6Byte1;
    _data[1] = _dataBroker->S_0B6Byte2;
    _data[2] = _dataBroker->S_0B6Byte3;
    _data[3] = _dataBroker->S_0B6Byte4;
    _data[4] = _dataBroker->S_0B6Byte5;
    _data[5] = _dataBroker->S_0B6Byte6;
    _data[6] = _dataBroker->S_0B6Byte7;
    _data[7] = _dataBroker->S_0B6Byte8;
    _wasDataSet = true;
}

