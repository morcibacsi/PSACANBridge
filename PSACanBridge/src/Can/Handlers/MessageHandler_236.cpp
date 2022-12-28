#include "MessageHandler_236.h"

void MessageHandler_236::SetData()
{
    _data[0] = 0x54;
    _data[1] = 0x03;
    _data[2] = 0xDE;
    _data[3] = 0x00;
    _data[4] = 0x00;
    _data[5] = 0x7F;
    _data[6] = 0xFE;
    _data[7] = 0x00;
}

uint8_t MessageHandler_236::SendMessage(unsigned long currentTime, bool forcedSend)
{
    if (_dataBroker->IgnitionDataArrived)
    {
        return ICanMessageHandler::SendMessage(currentTime, forcedSend);
    }
    return 0;
}
