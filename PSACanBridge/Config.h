#pragma once

#ifndef _Config_h
    #define _Config_h

#include <stdint.h>
#include <WString.h>

#include "Config_WifiAp.h"
//#include "Config_Bluetooth.h"
//#include "Config_WifiAp_Replay.h"

struct Config
{
    #define PRINT_DEBUG

    #define VIN_LENGTH 17

    #ifdef WIFI_ENABLED
        //#define WEBSOCKET_SERIAL
    #endif

    #ifndef WEBSOCKET_SERIAL
        // if defined messages are sent on bluetooth connection, otherwise standard serial is used
        #ifndef WIFI_ENABLED
            #define USE_BLUETOOTH_SERIAL
        #endif
    #endif

    const bool REPLAY_MODE = REPLAY_MODE_VALUE;

    bool MODIFY_217_WITH_CURRENT_SPEED = false;
    uint8_t CONSUMPTION_UNIT = 0;
    uint8_t DISTANCE_UNIT    = 0;
    uint8_t LANGUAGE         = 0b00001;
    uint8_t VOLUME_UNIT      = 0;
    uint8_t TEMPERATURE_UNIT = 0;
    uint8_t AMBIENCE_LEVEL   = 0b110;
    uint8_t SOUND_HARMONY    = 0b00;
    String  VERSION = "1.0.0";

    bool HAS_RTC                        = false;
    bool ORIGINAL_DISPLAY_PRESENT       = false;
    bool GENERATE_POPUP_FOR_DOOR_STATUS = false;

    // if true then the ignition signal is used from the CAN bus otherwise the ignition is considered as always on
    bool USE_IGNITION_SIGNAL_FROM_CAN_BUS = true;

    uint8_t FUEL_TANK_CAPACITY_IN_LITERS = 60;

    uint8_t VIN_FOR_HEADUNIT[VIN_LENGTH] = { 'V', 'F', '3', '0', 'E', 'R', 'H', 'H', 'A', 'C', 'S', '1', '2', '3', '4', '5', '6' };

    const bool WIFI_AP_MODE = WIFI_AP_MODE_VALUE;
    const char* WIFI_AP_NAME = WIFI_AP_NAME_VALUE;
    const char* WIFI_SSID = WIFI_SSID_VALUE;
    const char* WIFI_PASSWORD = WIFI_PASSWORD_VALUE;

    Config();
};

#endif
