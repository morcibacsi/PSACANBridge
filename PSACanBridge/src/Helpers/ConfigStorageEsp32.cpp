#include "ConfigStorageEsp32.h"
#include "../ESPFlash/ESPFlash.h"
#include "DebugPrint.h"

ConfigStorageEsp32::ConfigStorageEsp32(Config* config)
{
    _config = config;
}

void ConfigStorageEsp32::Remove()
{
    SPIFFS.begin(true);
    if (SPIFFS.exists(_fileName))
    {
        debug_println(F("config removed"));
        SPIFFS.remove(_fileName);
    }
    SPIFFS.end();
}

bool ConfigStorageEsp32::LoadFromDoc(DynamicJsonDocument doc)
{
    _config->MODIFY_217_WITH_CURRENT_SPEED    = doc["MODIFY_217_WITH_CURRENT_SPEED"];
    _config->CONSUMPTION_UNIT                 = doc["CONSUMPTION_UNIT"];
    _config->DISTANCE_UNIT                    = doc["DISTANCE_UNIT"];
    _config->LANGUAGE                         = doc["LANGUAGE"];
    _config->VOLUME_UNIT                      = doc["VOLUME_UNIT"];
    _config->TEMPERATURE_UNIT                 = doc["TEMPERATURE_UNIT"];
    _config->AMBIENCE_LEVEL                   = doc["AMBIENCE_LEVEL"];
    _config->SOUND_HARMONY                    = doc["SOUND_HARMONY"];
    _config->ORIGINAL_DISPLAY_PRESENT         = doc["ORIGINAL_DISPLAY_PRESENT"];
    _config->FUEL_TANK_CAPACITY_IN_LITERS     = doc["FUEL_TANK_CAPACITY_IN_LITERS"];
    _config->USE_IGNITION_SIGNAL_FROM_CAN_BUS = doc["USE_IGNITION_SIGNAL_FROM_CAN_BUS"];
    _config->GENERATE_POPUP_FOR_DOOR_STATUS   = doc["GENERATE_POPUP_FOR_DOOR_STATUS"];
    _config->HAS_RTC                          = doc["HAS_RTC"];

    JsonArray VIN_FOR_HEADUNIT = doc["VIN_FOR_HEADUNIT"];
    for (size_t i = 0; i < VIN_LENGTH; i++)
    {
        _config->VIN_FOR_HEADUNIT[i] = VIN_FOR_HEADUNIT[i];
    }

    //printArray1("Headunit vin in config: ", _config->VIN_FOR_HEADUNIT, VIN_LENGTH);
    return true;
}

bool ConfigStorageEsp32::Load()
{
    // Use the tool here: https://arduinojson.org/v6/assistant/
    debug_println(F("try to load config"));
    bool beginSuccess = SPIFFS.begin(true);
    if (beginSuccess)
    {
        if (SPIFFS.exists(_fileName))
        {
            File configFile = SPIFFS.open(_fileName, FILE_READ);
            DynamicJsonDocument doc(JSON_MEMORY_SIZE);
            DeserializationError error = deserializeJson(doc, configFile);
            if (error == DeserializationError::Ok)
            {
                debug_println(F("config loaded"));
                LoadFromDoc(doc);

                configFile.close();
            }
            else
            {
                //
                /*
                if (error == DeserializationError::EmptyInput)
                {
                    debug_println("Empty");
                }
                if (error == DeserializationError::IncompleteInput)
                {
                    debug_println("Incomplete");
                }
                if (error == DeserializationError::NoMemory)
                {
                    debug_println("NoMemory");
                }
                if (error == DeserializationError::InvalidInput)
                {
                    debug_println("Invalid");
                }
                if (error == DeserializationError::TooDeep)
                {
                    debug_println("TooDeep");
                }
                //*/
            }
        }
        else
        {
            debug_println(F("config does not exists"));
        }
        SPIFFS.end();
    }
    else
    {
        debug_println(F("SPIFFS begin failed!"));
    }

    return true;
}

StaticJsonDocument<JSON_MEMORY_SIZE> ConfigStorageEsp32::GetConfig()
{
    _configJson.clear();

    _configJson["MODIFY_217_WITH_CURRENT_SPEED"]    = _config->MODIFY_217_WITH_CURRENT_SPEED;
    _configJson["CONSUMPTION_UNIT"]                 = _config->CONSUMPTION_UNIT;
    _configJson["DISTANCE_UNIT"]                    = _config->DISTANCE_UNIT;
    _configJson["LANGUAGE"]                         = _config->LANGUAGE;
    _configJson["VOLUME_UNIT"]                      = _config->VOLUME_UNIT;
    _configJson["TEMPERATURE_UNIT"]                 = _config->TEMPERATURE_UNIT;
    _configJson["AMBIENCE_LEVEL"]                   = _config->AMBIENCE_LEVEL;
    _configJson["SOUND_HARMONY"]                    = _config->SOUND_HARMONY;
    _configJson["ORIGINAL_DISPLAY_PRESENT"]         = _config->ORIGINAL_DISPLAY_PRESENT;
    _configJson["FUEL_TANK_CAPACITY_IN_LITERS"]     = _config->FUEL_TANK_CAPACITY_IN_LITERS;
    _configJson["USE_IGNITION_SIGNAL_FROM_CAN_BUS"] = _config->USE_IGNITION_SIGNAL_FROM_CAN_BUS;
    _configJson["GENERATE_POPUP_FOR_DOOR_STATUS"]   = _config->GENERATE_POPUP_FOR_DOOR_STATUS;
    _configJson["HAS_RTC"]                          = _config->HAS_RTC;
    _configJson["VERSION"]                          = _config->VERSION;

    JsonArray VIN_FOR_HEADUNIT = _configJson.createNestedArray("VIN_FOR_HEADUNIT");

    for (size_t i = 0; i < VIN_LENGTH; i++)
    {
        VIN_FOR_HEADUNIT.add(_config->VIN_FOR_HEADUNIT[i]);
    }

    return _configJson;
}

bool ConfigStorageEsp32::Save()
{
    Remove();

    SPIFFS.begin(true);
    File configFile = SPIFFS.open(_fileName, FILE_WRITE);

    GetConfig();

    if (serializeJson(_configJson, configFile))
    {
        debug_println(F("config saved"));
    }
    else
    {
        debug_println(F("config not saved"));
    }

    configFile.close();
    SPIFFS.end();

    return true;
}
