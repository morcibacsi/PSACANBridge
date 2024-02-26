#pragma once

#ifndef _MessageHandler_1A8_h
    #define _MessageHandler_1A8_h

#include "ICanMessageHandler.h"
#include "../ICanMessageSender.h"
#include "../../Helpers/DataBroker.h"
#include <inttypes.h>

/*
    Cruise control related data, doesn't need to contain data, but the message need to be present
*/
class MessageHandler_1A8 : public ICanMessageHandler
{
    public:
    MessageHandler_1A8(ICanMessageSender* object, DataBroker *dataBroker): ICanMessageHandler(object, dataBroker, 0x1A8, 8, 200)
    {
        _wasDataSet = true;
    };

    void SetData() override;
};
#endif
