#include "MessageHandler_227.h"

void MessageHandler_227::SetData()
{
    _data[0] = _dataBroker->S_227Byte1;
    _data[1] = _dataBroker->S_227Byte2;
    _data[2] = _dataBroker->S_227Byte3;
    _data[3] = _dataBroker->S_227Byte4;
    _data[4] = 0x00;
    _data[5] = 0x00;
    _data[6] = 0x00;
    _data[7] = 0x00;

     _wasDataSet = true;
}
