#pragma once

#ifndef _MessageHandler_168_h
    #define _MessageHandler_168_h

#include "ICanMessageHandler.h"
#include "../ICanMessageSender.h"
#include "../../Helpers/DataBroker.h"
#include <inttypes.h>

class MessageHandler_168 : public ICanMessageHandler
{
    public:
    MessageHandler_168(ICanMessageSender* object, DataBroker *dataBroker): ICanMessageHandler(object, dataBroker, 0x168, 8, 200)
    {
    };

    void SetData() override;
};
#endif
