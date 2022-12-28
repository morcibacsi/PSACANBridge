#include "MessageHandler_221.h"

void MessageHandler_221::SetData()
{
    _data[0] = 0x00;
    _data[1] = _dataBroker->S_221Byte2 == 0xFF ? 0x00 : _dataBroker->S_221Byte2;
    _data[2] = _dataBroker->S_221Byte3 == 0xFF ? 0x00 : _dataBroker->S_221Byte3;
    _data[3] = _dataBroker->S_221Byte4 == 0xFF ? 0x00 : _dataBroker->S_221Byte4;
    _data[4] = _dataBroker->S_221Byte5 == 0xFF ? 0x00 : _dataBroker->S_221Byte5;
    _data[5] = _dataBroker->S_221Byte6 == 0xFF ? 0x00 : _dataBroker->S_221Byte6;
    _data[6] = _dataBroker->S_221Byte7 == 0xFF ? 0x00 : _dataBroker->S_221Byte7;

     _wasDataSet = true;
}
