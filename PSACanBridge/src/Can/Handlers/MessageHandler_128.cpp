#include "MessageHandler_128.h"
#include "../Structs/CAN_128_2010.h"
#include "../../Config.h"

void MessageHandler_128::SetData()
{
    Can2010CombineLightsByte1 field0;
    field0.data.right_turn_indicator    = _dataBroker->RightTurnIndicator;
    field0.data.left_turn_indicator     = _dataBroker->LeftTurnIndicator;
    field0.data.front_foglight          = _dataBroker->FrontFog;
    field0.data.rear_foglight           = _dataBroker->RearFog;
    field0.data.low_beam_on             = _dataBroker->LowBeam;
    field0.data.high_beam_on            = _dataBroker->HighBeam;
    field0.data.parking_light_indicator = _dataBroker->SideLights;
    field0.data.drl                     = _dataBroker->DRL;
//
/*
    Can2010CombineLightsByte2 field1;
    field1.data.gear_position            = _dataBroker->GearPosition;
    field1.data.gear_position_in_driving = _dataBroker->GearPositionDrive;
    field1.data.display_blinking         = _dataBroker->GearBlinking;

    Can2010CombineLightsByte3 field2;
    field2.data.auto_gearbox_mode      = _dataBroker->GearBoxMode;
    field2.data.auto_gearbox_selection = _dataBroker->GearBoxSelection;
//*/
    Can2010CombineLightsByte4 field3;
    field3.data.fse_inhibited                      = _dataBroker->FSEDisabled;
    field3.data.handbrake_signal                   = _dataBroker->Handbrake;
    field3.data.foot_on_break_indicator            = _dataBroker->FootOnBrake;
    field3.data.passenger_airbag_activated         = _dataBroker->PassengerAirbagDeactivated;
    field3.data.child_security_active              = _dataBroker->ChildSafetyActivated;
    field3.data.stop_light                         = _dataBroker->StopOnCMB;
    field3.data.service_indicator_exclamation      = _dataBroker->ServiceOnCMB;

    Can2010CombineLightsByte5 field4;
    field4.data.suspension_status                  = _dataBroker->SuspensionStatus;
    field4.data.esp_in_progress                    = _dataBroker->EspInRegulation;
    field4.data.esp_inactivated                    = _dataBroker->EspDeactivated;
    field4.data.operation_indicator_light_blinking = _dataBroker->SpaceMeasureBlinking;
    field4.data.operation_indicator_light_on       = _dataBroker->SpaceMeasureOn;

    field4.data.door_open                          = _dataBroker->DoorStatusByte > 0;
    field4.data.diesel_pre_heating                 = _dataBroker->PreHeatingStatus;

    Can2010CombineLightsByte6 field5;
    field5.data.row1_rl_seatbelt_forgotten          = _dataBroker->Row1RLSeatBelt;
    field5.data.scr_indicator                       = _dataBroker->SCRIndicator;
    field5.data.passenger_seatbelt_warning_blinking = _dataBroker->PassengerSeatbeltBlinking;
    field5.data.passenger_seatbelt_warning          = _dataBroker->PassengerSeatbeltWarning;
    field5.data.driver_seatbelt_warning             = _dataBroker->DriverSeatbeltWarning;
    field5.data.driver_seatbelt_warning_blinking    = _dataBroker->DriverSeatbeltWarningBlinking;
    field5.data.fuel_level_low                      = _dataBroker->FuelLow;

    Can2010CombineLightsByte7 field6;
    field6.data.activate_front_passenger_protection = _dataBroker->ActivatePassengerProtection;
    field6.data.warning_light_active                = _dataBroker->WarningLight;
    field6.data.cmb_active                          = _dataBroker->OdometerEnabled;
    field6.data.row1_rr_seatbelt_forgotten_blinking = _dataBroker->Row1RRSeatBeltBlinking;
    field6.data.row1_rr_seatbelt_forgotten          = _dataBroker->Row1RRSeatBelt;
    field6.data.row1_rc_seatbelt_forgotten_blinking = _dataBroker->Row1RCSeatBeltBlinking;
    field6.data.row1_rc_seatbelt_forgotten          = _dataBroker->Row1RCSeatBelt;
    field6.data.row1_rl_seatbelt_forgotten_blinking = _dataBroker->Row1RLSeatBeltBlinking;

    Can2010CombineLightsByte8 field7;
    field7.data.change_personalization              = _dataBroker->ChangePersonalization;
    field7.data.change_color                        = _dataBroker->ChangeColor;
    field7.data.ready_lamp                          = _dataBroker->Ready;
    field7.data.minc_blinking                       = _dataBroker->MINCBlinking;
    field7.data.foot_on_clutch                      = _dataBroker->FootOnClutch;

    _data[0] = field0.asByte;
    _data[1] = _dataBroker->S_128Byte7;//0x5f
    _data[2] = _dataBroker->S_128Byte8;
    _data[3] = field3.asByte;
    _data[4] = field4.asByte;
    _data[5] = field5.asByte;
    _data[6] = field6.asByte;
    _data[7] = field7.asByte;

    _wasDataSet = true;
}

