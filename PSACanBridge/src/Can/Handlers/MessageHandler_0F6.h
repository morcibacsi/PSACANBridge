#pragma once

#ifndef _MessageHandler_0F6_h
    #define _MessageHandler_0F6_h

#include "ICanMessageHandler.h"
#include "../ICanMessageSender.h"
#include "../../Helpers/DataBroker.h"
#include <inttypes.h>

class MessageHandler_0F6 : public ICanMessageHandler
{
    public:
    MessageHandler_0F6(ICanMessageSender* object, DataBroker *dataBroker): ICanMessageHandler(object, dataBroker, 0x0F6, 8, 500-50)
    {
        // Set external temperature to -- by default
        _data[0] = 0x88;
        //_data[6] = 0xFF;
    };

    void SetData() override;
};
#endif
