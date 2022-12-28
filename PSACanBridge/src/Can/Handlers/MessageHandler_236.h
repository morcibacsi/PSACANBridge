#pragma once

#ifndef _MessageHandler_236_h
    #define _MessageHandler_236_h

#include "ICanMessageHandler.h"
#include "../ICanMessageSender.h"
#include "../../Helpers/DataBroker.h"
#include <inttypes.h>

class MessageHandler_236 : public ICanMessageHandler
{
    public:
    MessageHandler_236(ICanMessageSender* object, DataBroker *dataBroker): ICanMessageHandler(object, dataBroker, 0x236, 8, 1000)
    {
        _wasDataSet = true;
    };

    void SetData() override;
    uint8_t SendMessage(unsigned long currentTime, bool forcedSend) override;
};
#endif
