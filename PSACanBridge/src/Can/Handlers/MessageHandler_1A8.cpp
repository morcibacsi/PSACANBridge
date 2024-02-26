#include "MessageHandler_1A8.h"

void MessageHandler_1A8::SetData()
{
    _wasDataSet = true;

    if (_dataBroker->CruiseControlActivateFunction)
    {
        _data[0] = 0x02;
    } else
    {
        _data[0] = 0x00;
    }

    _data[1] = _dataBroker->CruiseSpeed1;
    _data[2] = _dataBroker->CruiseSpeed2;

    _data[3] = _dataBroker->CruiseSpeed1;
    _data[4] = _dataBroker->CruiseSpeed2;

    _data[5] = _dataBroker->TripOnCMB.data.leftByte;
    _data[6] = _dataBroker->TripOnCMB.data.middleByte;
    _data[7] = _dataBroker->TripOnCMB.data.rightByte;
}
