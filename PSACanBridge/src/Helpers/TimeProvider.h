#pragma once

#ifndef _TimeProvider_h
    #define _TimeProvider_h

#include "../Helpers/DataBroker.h"
#include "../../Config.h"
#include <ErriezDS3231.h>
#include <Wire.h>

class TimeProvider {

Config *_config;
unsigned long _previousTime = 0;
DataBroker *_dataBroker;
ErriezDS3231 _rtc;
uint8_t _sdaPin;
uint8_t _sclPin;
bool _started = false;

public:
    TimeProvider(uint8_t sdaPin, uint8_t sclPin, DataBroker *dataBroker, Config *config);

    void Start();
    bool Process(unsigned long currentTime);
    void SetDateTime(uint16_t year,  uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second);
};

#endif
