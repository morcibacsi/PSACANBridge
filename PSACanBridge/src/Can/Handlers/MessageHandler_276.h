#pragma once

#ifndef _MessageHandler_276_h
    #define _MessageHandler_276_h

#include "ICanMessageHandler.h"
#include "../ICanMessageSender.h"
#include "../../Helpers/DataBroker.h"
#include "../../../Config.h"
#include <inttypes.h>

class MessageHandler_276 : public ICanMessageHandler
{
    Config* _config;
    public:
    MessageHandler_276(ICanMessageSender* object, DataBroker *dataBroker, Config* config): ICanMessageHandler(object, dataBroker, 0x276, 8, 1000)
    {
        _config = config;
        _wasDataSet = true;
    };

    void SetData() override;
};
#endif
