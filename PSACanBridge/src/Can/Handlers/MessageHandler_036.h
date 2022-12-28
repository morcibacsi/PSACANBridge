#pragma once

#ifndef _MessageHandler_036_h
    #define _MessageHandler_036_h

#include "ICanMessageHandler.h"
#include "../ICanMessageSender.h"
#include "../../Helpers/DataBroker.h"
#include "../../../Config.h"
#include <inttypes.h>

//2010 slight difference in almost all bytes, mostly compatible with 2004

/*
    Ignition data for EMF, radio, telematics, etc. Brightness for CMB
*/
class MessageHandler_036 : public ICanMessageHandler
{
    Config* _config;

    public:
    MessageHandler_036(ICanMessageSender* object, DataBroker *dataBroker, Config* config) : ICanMessageHandler(object, dataBroker, 0x036, 8, 100-10) {
        _config = config;

    _data[0] = 0x00;
    _data[1] = 0x00;
    _data[2] = 0x00;
    _data[3] = 0x00;
    _data[4] = 0x01;
    _data[5] = 0x00;
    _data[6] = 0x00;
    _data[7] = 0xA0;
    };

    void SetData() override;
};
#endif
