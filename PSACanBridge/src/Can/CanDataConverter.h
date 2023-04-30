#pragma once

#ifndef _CanDataSenderTask_h
    #define _CanDataSenderTask_h

#include "../../Config.h"

#include "ICanMessageSender.h"
#include "../SerialPort/AbstractSerial.h"

#include "CanMessageHandlerContainer2010.h"

#include "../Helpers/DataBroker.h"
#include "../Helpers/IntUnions.h"

class CanDataConverter {
    ICanMessageSender *_canMessageSender;
    AbsSer *_serial;
    UInt24 _mileage;

    uint8_t can2004Data[8];
    uint8_t _msgLength;

    Config *_config;
    DataBroker *_dataBroker;

    CanMessageHandlerContainer2010* _canMessageHandlerContainer2010;

    unsigned long _currentTime;

    public:
    CanDataConverter(ICanMessageSender *canMessageSender,
                      Config *config,
                      DataBroker *dataBroker,
                      AbsSer *serial,
                      CanMessageHandlerContainer2010 *canMessageHandlerContainer2010
                      );
    void SendCanMessage(unsigned long currentTime);
    void ProcessMessage2004(unsigned long currentTime, uint16_t canId, uint8_t canMessage[8], uint8_t msgLength);
    void Handle_036();
    void Handle_0F6();
    void Handle_217();
    void Handle_127();
    void Handle_128();
    void Handle_168();
    void Handle_3A7();
    void Handle_1A8();
    void Handle_361();
    void Handle_0E6();
    void Handle_161();
    void Handle_0B6();
    void Handle_227();
    void Handle_221();
    void Handle_220();
    void Handle_1A1();
    void Handle_21F();
};
#endif
