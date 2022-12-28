#include "MessageHandler_1A1.h"
#include "../Structs/CanDisplayStructs.h"
#include "Arduino.h"
void MessageHandler_1A1::SetData()
{
    bool isDoorOpen = _dataBroker->DoorStatusByte > 0;
    bool doorStateChanged = _prevDoorStatus != _dataBroker->DoorStatusByte;
    bool showDoorMessage =
        _dataBroker->Ignition &&
        _config->GENERATE_POPUP_FOR_DOOR_STATUS &&
        (_dataBroker->S_1A1Byte1 == 0xFF || _dataBroker->S_1A1Byte2 == 0xFF) &&
        (isDoorOpen || doorStateChanged);

    if (showDoorMessage)
    {
        if (doorStateChanged)
        {
            _data[0] = CAN_POPUP_MSG_HIDE;
            _data[1] = CAN_POPUP_MSG_CLEAR;
            _prevDoorStatus = _dataBroker->DoorStatusByte;
            return;
        }

        if (isDoorOpen)
        {
            CanDisplayByte2Struct field2;
            field2.data.priority = 1;
            field2.data.show_popup_on_cmb = 1;
            field2.data.show_popup_on_emf = 1;
            field2.data.show_popup_on_vth = 1;

            CanDisplayDoorStatus1Struct doorStatus;
            doorStatus.asByte = 0;
            doorStatus.data.is_front_left_open  = _dataBroker->IsFrontLeftDoorOpen;
            doorStatus.data.is_front_right_open = _dataBroker->IsFrontRightDoorOpen;
            doorStatus.data.is_rear_left_open   = _dataBroker->IsRearLeftDoorOpen;
            doorStatus.data.is_rear_right_open  = _dataBroker->IsRearRightDoorOpen;
            doorStatus.data.is_boot_open        = _dataBroker->IsBootLidOpen;
            doorStatus.data.is_bonnet_open      = _dataBroker->IsHoodOpen;

            CanDisplayDoorStatus2Struct doorStatus2;
            doorStatus2.asByte = 0;
            doorStatus2.data.is_fuel_flap_open = _dataBroker->IsFuelFlapOpen;

            _data[0] = CAN_POPUP_MSG_SHOW_CATEGORY1;
            _data[1] = CAN_POPUP_MSG_DOORS_BOOT_BONNET_REAR_SCREEN_AND_FUEL_TANK_OPEN;
            _data[2] = field2.asByte;
            _data[3] = doorStatus.asByte;
            _data[4] = doorStatus2.asByte;
        }
    }
    else
    {
        _data[0] = _dataBroker->S_1A1Byte1;
        _data[1] = _dataBroker->S_1A1Byte2;
        _data[2] = _dataBroker->S_1A1Byte3;
        _data[3] = _dataBroker->S_1A1Byte4;
        _data[4] = _dataBroker->S_1A1Byte5;
        _data[5] = _dataBroker->S_1A1Byte6;
        _data[6] = _dataBroker->S_1A1Byte7;
        _data[7] = _dataBroker->S_1A1Byte8;
    }

    _wasDataSet = true;
}

uint8_t MessageHandler_1A1::SendMessage(unsigned long currentTime, bool forcedSend)
{
    return ICanMessageHandler::SendMessage(currentTime, forcedSend);
}