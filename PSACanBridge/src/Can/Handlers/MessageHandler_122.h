#pragma once

#ifndef _MessageHandler_122_h
    #define _MessageHandler_122_h

#include "ICanMessageHandler.h"
#include "../ICanMessageSender.h"
#include "../../Helpers/DataBroker.h"
#include <inttypes.h>

/*
    Cruise control related data
*/
class MessageHandler_122 : public ICanMessageHandler
{
    public:
    MessageHandler_122(ICanMessageSender* object, DataBroker *dataBroker): ICanMessageHandler(object, dataBroker, 0x122, 8, 200)
    {
        _wasDataSet = true;
    };

    void SetData() override;
    uint8_t SendMessage(unsigned long currentTime, bool forcedSend) override;
};
#endif
