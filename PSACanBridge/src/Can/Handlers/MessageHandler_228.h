#pragma once

#ifndef _MessageHandler_228_h
    #define _MessageHandler_228_h

#include "ICanMessageHandler.h"
#include "../ICanMessageSender.h"
#include "../../Helpers/DataBroker.h"
#include <inttypes.h>

/*
    Cruise control related data
*/
class MessageHandler_228 : public ICanMessageHandler
{
    public:
    MessageHandler_228(ICanMessageSender* object, DataBroker *dataBroker): ICanMessageHandler(object, dataBroker, 0x228, 8, 1000)
    {
    };

    void SetData() override;
};
#endif
