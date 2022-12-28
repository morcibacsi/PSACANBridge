#pragma once

#ifndef _MessageHandler_217_h
    #define _MessageHandler_217_h

#include "ICanMessageHandler.h"
#include "../ICanMessageSender.h"
#include "../../Helpers/DataBroker.h"
#include <inttypes.h>

class MessageHandler_217 : public ICanMessageHandler
{
    public:
    MessageHandler_217(ICanMessageSender* object, DataBroker *dataBroker): ICanMessageHandler(object, dataBroker, 0x217, 8, 100)
    {
    };

    void SetData() override;
};
#endif
