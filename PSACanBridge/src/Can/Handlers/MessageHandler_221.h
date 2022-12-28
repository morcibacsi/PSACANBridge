#pragma once

#ifndef _MessageHandler_221_h
    #define _MessageHandler_221_h

#include "ICanMessageHandler.h"
#include "../ICanMessageSender.h"
#include "../../Helpers/DataBroker.h"
#include <inttypes.h>

class MessageHandler_221 : public ICanMessageHandler
{
    public:
    MessageHandler_221(ICanMessageSender* object, DataBroker *dataBroker): ICanMessageHandler(object, dataBroker, 0x221, 7, 1000)
    {
    };

    void SetData() override;
};
#endif
