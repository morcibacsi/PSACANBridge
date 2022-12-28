#pragma once

#ifndef _MessageHandler_3E7_h
    #define _MessageHandler_3E7_h

#include "ICanMessageHandler.h"
#include "../ICanMessageSender.h"
#include "../../Helpers/DataBroker.h"
#include <inttypes.h>

class MessageHandler_3E7 : public ICanMessageHandler
{
    public:
    MessageHandler_3E7(ICanMessageSender* object, DataBroker *dataBroker): ICanMessageHandler(object, dataBroker, 0x3E7, 5, 500)
    {
    };

    void SetData() override;
};
#endif
