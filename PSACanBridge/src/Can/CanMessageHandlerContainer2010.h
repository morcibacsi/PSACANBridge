#pragma once

#ifndef _CanMessageHandlerContainer2010_h
    #define _CanMessageHandlerContainer2010_h


#include "ICanMessageSender.h"
#include "Handlers/ICanMessageHandler.h"
#include "../../Config.h"
#include "../Helpers/DataBroker.h"
#include <inttypes.h>
#include <list>

class CanMessageHandlerContainer2010
{
    std::list<ICanMessageHandler*> _handlers;

    public:
    CanMessageHandlerContainer2010(ICanMessageSender *canInterface, Config *config, DataBroker *dataBroker);

    ICanMessageHandler* GetHandler(uint16_t canId);
    void SendDueMessages(unsigned long currentTime);
    void SendMessageForced(uint16_t canId, unsigned long currentTime);
    void SetData(uint16_t canId);
};
#endif
