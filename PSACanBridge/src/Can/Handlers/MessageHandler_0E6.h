#pragma once

#ifndef _MessageHandler_0E6_h
    #define _MessageHandler_0E6_h

#include "ICanMessageHandler.h"
#include "../ICanMessageSender.h"
#include "../../Helpers/DataBroker.h"
#include <inttypes.h>

class MessageHandler_0E6 : public ICanMessageHandler
{
    uint8_t chk_esp(uint8_t* buf);

    public:
    MessageHandler_0E6(ICanMessageSender* object, DataBroker *dataBroker): ICanMessageHandler(object, dataBroker, 0x0E6, 8, 100-10)
    {
    };

    void SetData() override;
    uint8_t SendMessage(unsigned long currentTime, bool forcedSend) override;
};
#endif
