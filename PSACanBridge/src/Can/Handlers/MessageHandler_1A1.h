#pragma once

#ifndef _MessageHandler_1A1_h
    #define _MessageHandler_1A1_h

#include "ICanMessageHandler.h"
#include "../ICanMessageSender.h"
#include "../../Helpers/DataBroker.h"
#include "../../../Config.h"
#include <inttypes.h>

class MessageHandler_1A1 : public ICanMessageHandler
{
    Config* _config;
    uint8_t _prevDoorStatus;
    public:
    MessageHandler_1A1(ICanMessageSender* object, DataBroker *dataBroker, Config* config): ICanMessageHandler(object, dataBroker, 0x1A1, 8, 200-10)
    {
        _config = config;
    };

    void SetData() override;
    uint8_t SendMessage(unsigned long currentTime, bool forcedSend) override;
};
#endif
