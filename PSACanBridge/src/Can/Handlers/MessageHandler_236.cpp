#include "MessageHandler_236.h"
#include "../Structs/CAN_236_2010.h"

void MessageHandler_236::SetData()
{
    CAN_236_2010_Byte1Struct byte1;
    byte1.data.vehicle_config = CAN_2010_VEHICLE_CONFIG_CLIENT;
    byte1.data.electric_network_status = CAN_2010_ELECTRIC_NETWORK_STATUS_NORMAL;

    if (_dataBroker->EconomyMode)
    {
        byte1.data.electric_network_status = CAN_2010_ELECTRIC_NETWORK_STATUS_ECO;
    }

    CAN_236_2010_Byte6Struct byte6;
    byte6.data.trunk_status = _dataBroker->IsBootLidOpen;
    byte6.data.driver_door_status = _dataBroker->IsFrontLeftDoorOpen;
    //byte6.data.brake_pedal_status = _dataBroker->FootOnBrake == 0 ? 1 : 0;

    CAN_236_2010_Byte8Struct byte8;
    byte8.data.passenger_door_status = _dataBroker->IsFrontRightDoorOpen;
    byte8.data.rear_left_door_status = _dataBroker->IsRearLeftDoorOpen;
    byte8.data.rear_right_door_status = _dataBroker->IsRearRightDoorOpen;
    byte8.data.driver_door_status_estimated = _dataBroker->IsFrontLeftDoorOpen;

    _data[0] = byte1.asByte;
    _data[1] = 0x03;
    _data[2] = 0xDE;
    _data[3] = 0x00;
    _data[4] = 0x00;
    _data[5] = byte6.asByte;
    _data[6] = 0xFE;
    _data[7] = byte8.asByte;
}

uint8_t MessageHandler_236::SendMessage(unsigned long currentTime, bool forcedSend)
{
    if (_dataBroker->IgnitionDataArrived)
    {
        return ICanMessageHandler::SendMessage(currentTime, forcedSend);
    }
    return 0;
}
