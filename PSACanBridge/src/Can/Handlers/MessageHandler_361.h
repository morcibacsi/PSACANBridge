#pragma once

#ifndef _MessageHandler_361_h
    #define _MessageHandler_361_h

#include "ICanMessageHandler.h"
#include "../ICanMessageSender.h"
#include "../../Helpers/DataBroker.h"
#include <inttypes.h>

class MessageHandler_361 : public ICanMessageHandler
{
    public:
    MessageHandler_361(ICanMessageSender* object, DataBroker *dataBroker): ICanMessageHandler(object, dataBroker, 0x361, 7, 500)
    {
    };

    void SetData() override;
};
#endif
