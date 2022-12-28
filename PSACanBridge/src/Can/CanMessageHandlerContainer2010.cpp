#include "CanMessageHandlerContainer2010.h"
#include "Handlers/MessageHandler_0B6.h"
#include "Handlers/MessageHandler_0E6.h"
#include "Handlers/MessageHandler_0F6.h"
#include "Handlers/MessageHandler_1A1.h"
#include "Handlers/MessageHandler_1A8.h"
#include "Handlers/MessageHandler_2B6.h"
#include "Handlers/MessageHandler_3B6.h"
#include "Handlers/MessageHandler_3E7.h"
#include "Handlers/MessageHandler_036.h"
#include "Handlers/MessageHandler_122.h"
#include "Handlers/MessageHandler_128.h"
#include "Handlers/MessageHandler_161.h"
#include "Handlers/MessageHandler_168.h"
#include "Handlers/MessageHandler_217.h"
#include "Handlers/MessageHandler_221.h"
#include "Handlers/MessageHandler_227.h"
#include "Handlers/MessageHandler_228.h"
#include "Handlers/MessageHandler_236.h"
#include "Handlers/MessageHandler_260.h"
#include "Handlers/MessageHandler_276.h"
#include "Handlers/MessageHandler_336.h"
#include "Handlers/MessageHandler_361.h"

CanMessageHandlerContainer2010::CanMessageHandlerContainer2010(ICanMessageSender *canInterface, Config *config, DataBroker *dataBroker)
{
    _handlers.push_back(new MessageHandler_0B6(canInterface, dataBroker));
    _handlers.push_back(new MessageHandler_0E6(canInterface, dataBroker));
    _handlers.push_back(new MessageHandler_1A1(canInterface, dataBroker, config));
    _handlers.push_back(new MessageHandler_1A8(canInterface, dataBroker));
    _handlers.push_back(new MessageHandler_0F6(canInterface, dataBroker));
    _handlers.push_back(new MessageHandler_2B6(canInterface, dataBroker, config));
    _handlers.push_back(new MessageHandler_3B6(canInterface, dataBroker, config));
    _handlers.push_back(new MessageHandler_3E7(canInterface, dataBroker));
    _handlers.push_back(new MessageHandler_036(canInterface, dataBroker, config));
    _handlers.push_back(new MessageHandler_122(canInterface, dataBroker));
    _handlers.push_back(new MessageHandler_128(canInterface, dataBroker, config));
    _handlers.push_back(new MessageHandler_161(canInterface, dataBroker, config));
    _handlers.push_back(new MessageHandler_168(canInterface, dataBroker));
    _handlers.push_back(new MessageHandler_217(canInterface, dataBroker));
    //_handlers.push_back(new MessageHandler_221(canInterface, dataBroker));
    _handlers.push_back(new MessageHandler_227(canInterface, dataBroker));
    _handlers.push_back(new MessageHandler_228(canInterface, dataBroker));
    _handlers.push_back(new MessageHandler_236(canInterface, dataBroker));
    _handlers.push_back(new MessageHandler_260(canInterface, dataBroker, config));
    _handlers.push_back(new MessageHandler_276(canInterface, dataBroker, config));
    _handlers.push_back(new MessageHandler_336(canInterface, dataBroker, config));
    _handlers.push_back(new MessageHandler_361(canInterface, dataBroker));
}

ICanMessageHandler* CanMessageHandlerContainer2010::GetHandler(uint16_t canId)
{
    for (ICanMessageHandler* handler : _handlers)
    {
        if (handler->GetCanId() == canId)
        {
            return handler;
        }
    }

    return nullptr;
}

void CanMessageHandlerContainer2010::SendDueMessages(unsigned long currentTime)
{
    for (ICanMessageHandler* handler : _handlers)
    {
        handler->SendMessage(currentTime, false);
    }
}

void CanMessageHandlerContainer2010::SendMessageForced(uint16_t canId, unsigned long currentTime)
{
    ICanMessageHandler* handler = GetHandler(canId);

    if (handler != nullptr)
    {
        handler->SendMessage(currentTime, true);
    }
}

void CanMessageHandlerContainer2010::SetData(uint16_t canId)
{
    ICanMessageHandler* handler = GetHandler(canId);

    if (handler != nullptr)
    {
        handler->SetData();
    }
}