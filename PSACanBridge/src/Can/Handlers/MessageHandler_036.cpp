#include "MessageHandler_036.h"
#include "../Structs/CAN_036.h"
#include "../Structs/CAN_036_2010.h"

void MessageHandler_036::SetData()
{
    CAN_036_2010_Byte1Struct byte1;
    byte1.data.driving_direction = _dataBroker->IsReverseEngaged ? DRIVING_DIRECTION_REVERSE : DRIVING_DIRECTION_FORWARD;

    CAN_036_2010_Byte3Struct byte3;
    byte3.data.economy_mode_active = _dataBroker->EconomyMode;

    CAN_036_2010_Byte4Struct byte4;
    byte4.data.night_mode = _dataBroker->NightMode;
    byte4.data.dashboard_brightness = _dataBroker->Brightness;
    byte4.data.black_panel_status = _dataBroker->BlackPanel;

    CAN_036_2010_Byte5Struct byte5;
    byte5.data.ignition_mode = _dataBroker->IgnitionMode;

    _data[0] = byte1.asByte;
    _data[2] = byte3.asByte;
    _data[3] = byte4.asByte;
    _data[4] = byte5.asByte;
    _data[7] = 0xA0;

    _wasDataSet = true;
}
