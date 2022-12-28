#pragma once

#ifndef _MessageHandler_161_h
    #define _MessageHandler_161_h

#include "ICanMessageHandler.h"
#include "../ICanMessageSender.h"
#include "../../Helpers/DataBroker.h"
#include "../../../Config.h"
#include <inttypes.h>

class MessageHandler_161 : public ICanMessageHandler
{
    Config *_config;
    public:
    MessageHandler_161(ICanMessageSender* object, DataBroker *dataBroker, Config *config): ICanMessageHandler(object, dataBroker, 0x161, 7, 500-20)
    {
        _config = config;
    };

    void SetData() override;
};
#endif
