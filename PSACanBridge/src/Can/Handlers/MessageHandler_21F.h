#pragma once

#ifndef _MessageHandler_21F_h
    #define _MessageHandler_21F_h

#include "ICanMessageHandler.h"
#include "../ICanMessageSender.h"
#include "../../Helpers/DataBroker.h"
#include "../../../Config.h"
#include <inttypes.h>

class MessageHandler_21F : public ICanMessageHandler
{
    Config* _config;
    public:
    MessageHandler_21F(ICanMessageSender* object, DataBroker *dataBroker, Config* config): ICanMessageHandler(object, dataBroker, 0x21F, 3, 100)
    {
        _config = config;
    };

    void SetData() override;
};
#endif
