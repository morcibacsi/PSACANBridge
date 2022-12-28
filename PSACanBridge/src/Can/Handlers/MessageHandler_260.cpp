#include "MessageHandler_260.h"
#include "../Structs/CAN_260_2010.h"

void MessageHandler_260::SetData()
{
    CAN_260_2010_Byte1Struct byte1;
    byte1.data.consumption_unit = _config->CONSUMPTION_UNIT;
    byte1.data.distance_unit    = _config->DISTANCE_UNIT;
    byte1.data.language         = _config->LANGUAGE;
    byte1.data.unit_and_language_data_valid = 1;

    CAN_260_2010_Byte2Struct byte2;
    byte2.data.volume_unit      = _config->VOLUME_UNIT;
    byte2.data.temperature_unit = _config->TEMPERATURE_UNIT;
    byte2.data.ambience_level   = _config->AMBIENCE_LEVEL;
    byte2.data.sound_harmony    = _config->SOUND_HARMONY;
    byte2.data.vehicle_function_data = 1;

    CAN_260_2010_Byte3Struct byte3;
    byte3.data.ambience_lighting        = _dataBroker->AmbientLighting;
    byte3.data.drl                      = _dataBroker->DRL;
    byte3.data.automatic_electric_brake = _dataBroker->AutomaticElectricBrake;

    CAN_260_2010_Byte6Struct byte6;
    byte6.data.braking_on_alarm_risk = _dataBroker->BreakingOnAlarmRisk;

    _data[0] = byte1.asByte;
    _data[1] = byte2.asByte;
    _data[2] = byte3.asByte;

    _data[3] = 0x00;
    _data[4] = 0x00;
    _data[5] = byte6.asByte;
    _data[6] = 0x00;
    _data[7] = 0x00;

     _wasDataSet = true;
}

uint8_t MessageHandler_260::SendMessage(unsigned long currentTime, bool forcedSend)
{
    if (_dataBroker->IgnitionDataArrived)
    {
        return ICanMessageHandler::SendMessage(currentTime, forcedSend);
    }
    return 0;
}
