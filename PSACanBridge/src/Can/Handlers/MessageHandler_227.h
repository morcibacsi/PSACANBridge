#pragma once

#ifndef _MessageHandler_227_h
    #define _MessageHandler_227_h

#include "ICanMessageHandler.h"
#include "../ICanMessageSender.h"
#include "../../Helpers/DataBroker.h"
#include <inttypes.h>

class MessageHandler_227 : public ICanMessageHandler
{
    public:
    MessageHandler_227(ICanMessageSender* object, DataBroker *dataBroker): ICanMessageHandler(object, dataBroker, 0x227, 7, 500)
    {
    };

    void SetData() override;
};
#endif
