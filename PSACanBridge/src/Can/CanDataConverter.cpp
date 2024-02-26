#include "CanDataConverter.h"
#include <string.h>

#include "../Can/Structs/CAN_036.h"
#include "../Can/Structs/CAN_0F6.h"
#include "../Can/Structs/CAN_217.h"
#include "../Can/Structs/CAN_127.h"
#include "../Can/Structs/CAN_128.h"
#include "../Can/Structs/CAN_168.h"
#include "../Can/Structs/CAN_3A7.h"
#include "../Can/Structs/CAN_1A8_2004.h"
#include "../Can/Structs/CAN_361_2004.h"
#include "../Can/Structs/CAN_220.h"
#include "../Helpers/DebugPrint.h"
#include "../Helpers/IntUnions.h"

CanDataConverter::CanDataConverter(
                        ICanMessageSender *canMessageSender,
                        Config *config,
                        DataBroker *dataBroker,
                        AbsSer *serial,
                        CanMessageHandlerContainer2010 *canMessageHandlerContainer2010
                      )

{
    _canMessageHandlerContainer2010 = canMessageHandlerContainer2010;

    _canMessageSender = canMessageSender;
    _dataBroker = dataBroker;
    _serial = serial;

    _config = config;

    if (!_config->USE_IGNITION_SIGNAL_FROM_CAN_BUS)
    {
        _dataBroker->Ignition = 1;
        _dataBroker->IgnitionDataArrived = true;

        _canMessageHandlerContainer2010->SetData(0x036);
        _canMessageHandlerContainer2010->SetData(0x0F6);
    }

    //These messages are sent without any incoming messages, so we need to initialize them
    _canMessageHandlerContainer2010->SetData(0x260);
    _canMessageHandlerContainer2010->SetData(0x236);
}

void IRAM_ATTR CanDataConverter::ProcessMessage2004(unsigned long currentTime, uint16_t canId, uint8_t canMessage[8], uint8_t msgLength)
{
    _currentTime = currentTime;
    _msgLength = msgLength;

    //make a copy of the buffer
    memcpy(can2004Data, canMessage, msgLength);

    switch (canId)
    {
        case 0x036:
            Handle_036();
            break;
        case 0x0F6:
            Handle_0F6();
            break;
        case 0x217:
            Handle_217();
            break;
        case 0x128:
            Handle_128();
            break;
        case 0x168:
            Handle_168();
            break;
        case 0x3A7:
            Handle_3A7();
        case 0x0E6:
            Handle_0E6();
            break;
        case 0x161:
            Handle_161();
            break;
        case 0x0B6:
            Handle_0B6();
            break;
        case 0x1A1:
            Handle_1A1();
            break;
        case 0x1A8:
            Handle_1A8();
            break;
        case 0x220:
            Handle_220();
            break;
        case 0x127:
            Handle_127();
            break;
        case 0x21F:
            Handle_21F();
            break;
        default:
            break;
    }
    //_serial->println(millis() - stopwatch);
}

void CanDataConverter::SendCanMessage(unsigned long currentTime)
{

}

void IRAM_ATTR CanDataConverter::Handle_036()
{
    CanIgnitionStruct tmp;
    memcpy(&tmp, can2004Data, sizeof(tmp));

    uint8_t ignitionMode = CAN_IGNITION_MODE_NORMAL;

    _dataBroker->IgnitionDataArrived = true;
    _dataBroker->EconomyMode = tmp.LoadShedding.data.economy_mode_active;
    _dataBroker->Brightness  = tmp.Brightness.data.dashboard_brightness;
    _dataBroker->NightMode   = tmp.Brightness.data.night_mode;
    _dataBroker->BlackPanel  = tmp.Brightness.data.black_panel_status;
    if (_config->USE_IGNITION_SIGNAL_FROM_CAN_BUS)
    {
        ignitionMode = tmp.Ignition.data.ignition_mode;
    }

    _dataBroker->IgnitionMode = ignitionMode;

   _canMessageHandlerContainer2010->SetData(0x036);
}

void IRAM_ATTR CanDataConverter::Handle_0F6()
{
    Can0F6Dash1Struct tmp;
    memcpy(&tmp, can2004Data, sizeof(tmp));

    _dataBroker->IsReverseEngaged    = tmp.LightsStatus.data.reverse_gear_light;
    _dataBroker->ExternalTemperature = tmp.ExternalTemperature;
    _dataBroker->CoolantTemperature  = tmp.CoolantTemperature;

    if (_config->USE_IGNITION_SIGNAL_FROM_CAN_BUS){
        _dataBroker->Ignition = tmp.IgnitionField.data.ignition;
    }
    else
    {
        _dataBroker->Ignition = 1;
    }

    //identical, we just needed the above data
    _dataBroker->S_0F6Byte1 = can2004Data[0];
    _dataBroker->S_0F6Byte2 = can2004Data[1];
    _dataBroker->S_0F6Byte3 = can2004Data[2];
    _dataBroker->S_0F6Byte4 = can2004Data[3];
    _dataBroker->S_0F6Byte5 = can2004Data[4];
    _dataBroker->S_0F6Byte6 = can2004Data[5];
    _dataBroker->S_0F6Byte7 = can2004Data[6];
    _dataBroker->S_0F6Byte8 = can2004Data[7];


    _canMessageHandlerContainer2010->SetData(0x036);
    _canMessageHandlerContainer2010->SetData(0x0F6);
}

void IRAM_ATTR CanDataConverter::Handle_217()
{
    //almost identical, only byte 8 is different
    _dataBroker->S_217Byte1 = can2004Data[0];
    _dataBroker->S_217Byte2 = can2004Data[1];
    _dataBroker->S_217Byte3 = can2004Data[2];
    _dataBroker->S_217Byte4 = can2004Data[3];
    _dataBroker->S_217Byte5 = can2004Data[4];
    _dataBroker->S_217Byte6 = can2004Data[5];
    _dataBroker->S_217Byte7 = can2004Data[6];
    _dataBroker->S_217Byte8 = 0;

    if (_config->MODIFY_217_WITH_CURRENT_SPEED)
    {
        _dataBroker->S_217Byte6 = _dataBroker->SpeedInKmh;
    }
    _canMessageHandlerContainer2010->SetData(0x217);
    _canMessageHandlerContainer2010->SendMessageForced(0x217, _currentTime);
}

void IRAM_ATTR CanDataConverter::Handle_128()
{
    CMB2004_128Struct tmp;
    memcpy(&tmp, can2004Data, sizeof(tmp));

    _dataBroker->DRL                          = tmp.Indicator5.data.drl;
    _dataBroker->LeftTurnIndicator            = tmp.Indicator5.data.left_turn_indicator;
    _dataBroker->RightTurnIndicator           = tmp.Indicator5.data.right_turn_indicator;
    _dataBroker->RearFog                      = tmp.Indicator5.data.rear_foglight;
    _dataBroker->FrontFog                     = tmp.Indicator5.data.front_foglight;
    _dataBroker->HighBeam                     = tmp.Indicator5.data.high_beam_on;
    _dataBroker->LowBeam                      = tmp.Indicator5.data.low_beam_on;
    _dataBroker->SideLights                   = tmp.Indicator5.data.parking_light_indicator;

    _dataBroker->S_128Byte7                   = can2004Data[6];
    _dataBroker->S_128Byte8                   = can2004Data[7];

    _dataBroker->GearPosition                 = tmp.Indicator7.data.gear_position_drive;

    _dataBroker->FSEDisabled                  = tmp.Indicator6.data.fse_inhibited;
    _dataBroker->Handbrake                    = tmp.Indicator1.data.handbrake_signal;
    _dataBroker->FootOnBrake                  = tmp.Indicator4.data.foot_on_break_indicator;
    _dataBroker->PassengerAirbagDeactivated   = tmp.Indicator1.data.passenger_airbag_deactivated;
    _dataBroker->ChildSafetyActivated         = tmp.Indicator3.data.child_security_active;
    _dataBroker->StopOnCMB                    = tmp.Indicator2.data.stop_light;
    _dataBroker->ServiceOnCMB                 = tmp.Indicator2.data.service_indicator_exclamation;

    _dataBroker->SuspensionStatus             = tmp.Indicator3.data.suspension_status;
    _dataBroker->EspInRegulation              = tmp.Indicator3.data.esp_in_progress;
    _dataBroker->EspDeactivated               = tmp.Indicator3.data.esp_inactivated;
    _dataBroker->SpaceMeasureBlinking         = tmp.Indicator4.data.operation_indicator_light_blinking;
    _dataBroker->SpaceMeasureOn               = tmp.Indicator4.data.operation_indicator_light_on;
    _dataBroker->PreHeatingStatus             = tmp.Indicator1.data.diesel_pre_heating;

    _dataBroker->Row1RLSeatBelt               = tmp.Indicator6.data.row1_rl_seatbelt_forgotten;
    _dataBroker->PassengerSeatbeltBlinking    = tmp.Indicator4.data.passenger_seatbelt_warning_blinking;
    _dataBroker->PassengerSeatbeltWarning     = tmp.Indicator1.data.passenger_seatbelt_warning;
    _dataBroker->DriverSeatbeltWarning        = tmp.Indicator1.data.driver_seatbelt_warning;
    _dataBroker->DriverSeatbeltWarningBlinking= tmp.Indicator4.data.driver_seatbelt_warning_blinking;
    _dataBroker->FuelLow                      = tmp.Indicator1.data.fuel_level_low;

    _dataBroker->ActivatePassengerProtection  = tmp.Indicator2.data.activate_front_passenger_protection;
    _dataBroker->WarningLight                 = tmp.Indicator3.data.warning_light_active;
    _dataBroker->OdometerEnabled              = tmp.Indicator6.data.cmb_active;
    _dataBroker->Row1RRSeatBeltBlinking       = tmp.Indicator6.data.row1_rr_seatbelt_forgotten_blinking;
    _dataBroker->Row1RRSeatBelt               = tmp.Indicator6.data.row1_rr_seatbelt_forgotten;
    _dataBroker->Row1RCSeatBeltBlinking       = tmp.Indicator6.data.row1_rc_seatbelt_forgotten_blinking;
    _dataBroker->Row1RCSeatBelt               = tmp.Indicator6.data.row1_rc_seatbelt_forgotten;
    _dataBroker->Row1RLSeatBeltBlinking       = tmp.Indicator6.data.row1_rl_seatbelt_forgotten_blinking;

    _dataBroker->ChangePersonalization        = tmp.Indicator3.data.change_personalization;
    _dataBroker->ChangeColor                  = tmp.Indicator3.data.change_color;
    _dataBroker->Ready                        = tmp.Indicator3.data.ready_lamp;

    _canMessageHandlerContainer2010->SetData(0x128);
    _canMessageHandlerContainer2010->SendMessageForced(0x128, _currentTime);
}

void IRAM_ATTR CanDataConverter::Handle_168()
{
    CanDash3Struct tmp;
    memcpy(&tmp, can2004Data, sizeof(tmp));

    _dataBroker->S_168Byte1             = can2004Data[0];
    _dataBroker->EngineSpeedThreshold2  = tmp.Field2.data.max_rpm_2;
    _dataBroker->EngineSpeedThreshold1  = tmp.Field2.data.max_rpm_1;
    _dataBroker->AutoWipingActive       = tmp.Field2.data.auto_wiping_active;
    _dataBroker->FapClogged             = tmp.Field2.data.fap_clogged;
    _dataBroker->FlatTyreAlert          = tmp.Field2.data.flat_tyre_alert;
    _dataBroker->TyrePressureAlert      = tmp.Field2.data.tyre_pressure_alert;

    _dataBroker->GeneratorFault         = tmp.Field5.data.generator_fault;
    _dataBroker->BatteryChargeFault     = tmp.Field5.data.battery_charge_fault;
    _dataBroker->SeriousSuspensionFault = tmp.Field4.data.serious_suspension_fault;
    _dataBroker->SeriousREFFault        = tmp.Field4.data.serious_ref_ehb_fault;

    _dataBroker->Mil                    = tmp.Field4.data.mil;
    _dataBroker->BrakePadFault          = tmp.Field4.data.brake_pad_fault;
    _dataBroker->AutoGearboxFault       = tmp.Field4.data.gearbox_fault;
    _dataBroker->EspFault               = tmp.Field4.data.esp_fault;
    _dataBroker->AbsFault               = tmp.Field4.data.abs_fault;
    _dataBroker->FSETighteningFault     = tmp.Field7.data.fse_tightening_fault;
    _dataBroker->FSESystemFault         = tmp.Field7.data.fse_system_fault;

    _dataBroker->STTLampStatus          = tmp.Field7.data.stt_lamp_status;
    _dataBroker->PowerSteeringFault     = tmp.Field7.data.power_steering_fault;
    _dataBroker->CAARLampStatus         = tmp.Field6.data.caar_lamp_status;
    _dataBroker->CurveCodeFault         = tmp.Field6.data.curve_code_fault;
    _dataBroker->WaterInDiesel          = tmp.Field4.data.water_in_diesel;

    _dataBroker->MINCBlinking           = tmp.Field2.data.minc_blinking;
    _dataBroker->FootOnClutch           = tmp.Field3.data.foot_on_clutch;
    _dataBroker->Row2RRSeatBelt         = tmp.Field3.data.row2_rr_seatbelt_forgotten;
    _dataBroker->Row2RRSeatBeltBlinking = tmp.Field3.data.row2_rr_seatbelt_forgotten_blinking;
    _dataBroker->Row2RCSeatBelt         = tmp.Field3.data.row2_rc_seatbelt_forgotten;
    _dataBroker->Row2RCSeatBeltBlinking = tmp.Field3.data.row2_rc_seatbelt_forgotten_blinking;
    _dataBroker->Row2RLSeatBelt         = tmp.Field3.data.row2_rl_seatbelt_forgotten;
    _dataBroker->Row2RLSeatBeltBlinking = tmp.Field3.data.row2_rl_seatbelt_forgotten_blinking;
    _dataBroker->SCRIndicator           = tmp.Field5.data.scr_indicator;

    _canMessageHandlerContainer2010->SetData(0x128);

    _canMessageHandlerContainer2010->SetData(0x168);
    _canMessageHandlerContainer2010->SendMessageForced(0x168, _currentTime);

}

void IRAM_ATTR CanDataConverter::Handle_3A7()
{
    Can3A7Struct tmp;
    memcpy(&tmp, can2004Data, sizeof(tmp));

    _dataBroker->IsMaintenanceDue          = tmp.MaintenanceType.data.maintenance_due;
    _dataBroker->WrenchIcon                = tmp.MaintenanceType.data.wrench_with_km;
    _dataBroker->MaintenanceSignKm         = tmp.KmMaintenanceInfo.data.maintenance_sign_km;
    _dataBroker->MaintenanceSignTime       = tmp.TemporaryMaintenanceInfo.data.maintenance_sign_time;
    _dataBroker->MaintenanceSignKmBlinking = tmp.KmMaintenanceInfo.data.km_blinking;
    _dataBroker->MaintenanceSignTime       = tmp.TemporaryMaintenanceInfo.data.time_blinking;

    _dataBroker->MaintenanceKilometers.data.leftByte  = tmp.MaintenanceKmByte1;
    _dataBroker->MaintenanceKilometers.data.rightByte = tmp.MaintenanceKmByte2;

    _dataBroker->MaintenanceTime.data.leftByte  = tmp.NumberOfDaysBeforeMaintenanceByte1;
    _dataBroker->MaintenanceTime.data.rightByte = tmp.NumberOfDaysBeforeMaintenanceByte2;

    _canMessageHandlerContainer2010->SetData(0x3E7);
}

void IRAM_ATTR CanDataConverter::Handle_1A8()
{
    CAN_1A8_2004Struct tmp;
    memcpy(&tmp, can2004Data, sizeof(tmp));

    _dataBroker->CruiseSpeed1 = can2004Data[1];
    _dataBroker->CruiseSpeed2 = can2004Data[2];
    //_dataBroker->CruiseByte1 = can2004Data[1];
    //_dataBroker->CruiseByte2 = can2004Data[2];

    //_dataBroker->CruiseControlSpeed.data.leftByte      = tmp.CreuiseControlSpeed1;
    //_dataBroker->CruiseControlSpeed.data.rightByte     = tmp.CruiseControlSpeed2;

    _dataBroker->CruiseControlActivateFunction         = tmp.Field1.data.activate_function;
    _dataBroker->CruiseControlSelectedFunction         = tmp.Field1.data.selected_function;
    _dataBroker->CruiseControlStatusOfSelectedFunction = tmp.Field1.data.status_of_selected_function;
    _dataBroker->TripOnCMB.data.leftByte               = can2004Data[5];
    _dataBroker->TripOnCMB.data.middleByte             = can2004Data[6];
    _dataBroker->TripOnCMB.data.rightByte              = can2004Data[7];


    _canMessageHandlerContainer2010->SetData(0x1A8);
    _canMessageHandlerContainer2010->SendMessageForced(0x1A8, _currentTime);

    _canMessageHandlerContainer2010->SetData(0x228);
    _canMessageHandlerContainer2010->SendMessageForced(0x228, _currentTime);
}

void IRAM_ATTR CanDataConverter::Handle_361()
{
    Can361_2004Struct tmp;
    memcpy(&tmp, can2004Data, sizeof(tmp));
    _dataBroker->AasDisable             = tmp.Field5.data.aas_disable;
    _dataBroker->AmbientLighting        = tmp.Field5.data.ambient_lighting;
    _dataBroker->AutoLighting           = tmp.Field3.data.auto_lighting;
    _dataBroker->AutomaticElectricBrake = tmp.Field3.data.automatic_electric_brake;
    _dataBroker->BlindspotMonitoring    = tmp.Field5.data.blindspot_monitoring;
    _dataBroker->BreakingOnAlarmRisk    = tmp.Field6.data.breaking_on_alarm_risk;
    _dataBroker->ConfigEnabled          = tmp.Field2.data.config_enabled;
    _dataBroker->DrlPresent             = tmp.Field4.data.drl_present;
    _dataBroker->FollowMeHome           = tmp.Field3.data.follow_me_home;
    _dataBroker->HighwayLightingPresent = tmp.Field5.data.highway_lighting_present;
    _dataBroker->HingePanelSelect       = tmp.Field3.data.hinge_panel_select;
    _dataBroker->IrcPresent             = tmp.Field3.data.irc_present;
    _dataBroker->PermanentRearFlapLock  = tmp.Field2.data.permanent_rear_flap_lock;
    _dataBroker->RearWiperOption        = tmp.Field4.data.rear_wiper_option;
    _dataBroker->TnbPresent             = tmp.Field6.data.tnb_present;
    _dataBroker->TpmsPresent            = tmp.Field6.data.tpms_present;
    _dataBroker->TpmsResetPresent       = tmp.Field6.data.tpms_reset_present;

    _canMessageHandlerContainer2010->SetData(0x361);
}

void IRAM_ATTR CanDataConverter::Handle_0E6()
{
    _dataBroker->S_0E6Byte1 = can2004Data[0];
    _dataBroker->S_0E6Byte2 = can2004Data[1];
    _dataBroker->S_0E6Byte3 = can2004Data[2];
    _dataBroker->S_0E6Byte4 = can2004Data[3];
    _dataBroker->S_0E6Byte5 = can2004Data[4];

    _canMessageHandlerContainer2010->SetData(0x0E6);
}

void IRAM_ATTR CanDataConverter::Handle_161()
{
    _dataBroker->S_161Byte1 = can2004Data[0];
    _dataBroker->S_161Byte3 = can2004Data[2];
    _dataBroker->S_161Byte4 = can2004Data[3];
    _dataBroker->S_161Byte7 = can2004Data[6];

    _canMessageHandlerContainer2010->SetData(0x161);
}

void IRAM_ATTR CanDataConverter::Handle_0B6()
{
    _dataBroker->S_0B6Byte1 = can2004Data[0];
    _dataBroker->S_0B6Byte2 = can2004Data[1];
    _dataBroker->S_0B6Byte3 = can2004Data[2];
    _dataBroker->S_0B6Byte4 = can2004Data[3];
    _dataBroker->S_0B6Byte5 = can2004Data[4];
    _dataBroker->S_0B6Byte6 = can2004Data[5];
    _dataBroker->S_0B6Byte7 = can2004Data[6];
    _dataBroker->S_0B6Byte8 = can2004Data[7];

    UInt16 speed;
    speed.data.leftByte = can2004Data[2];
    speed.data.rightByte = can2004Data[3];

    _dataBroker->SpeedInKmh = speed.asUint16;

    _canMessageHandlerContainer2010->SetData(0x0B6);
}

void IRAM_ATTR CanDataConverter::Handle_227()
{
    _dataBroker->S_227Byte1 = can2004Data[0];
    _dataBroker->S_227Byte2 = can2004Data[1];
    _dataBroker->S_227Byte3 = can2004Data[2];
    _dataBroker->S_227Byte4 = can2004Data[3];

    _canMessageHandlerContainer2010->SetData(0x227);
}

void IRAM_ATTR CanDataConverter::Handle_221()
{
    _dataBroker->S_221Byte1 = can2004Data[0];
    _dataBroker->S_221Byte2 = can2004Data[1];
    _dataBroker->S_221Byte3 = can2004Data[2];
    _dataBroker->S_221Byte4 = can2004Data[3];
    _dataBroker->S_221Byte5 = can2004Data[4];
    _dataBroker->S_221Byte6 = can2004Data[5];
    _dataBroker->S_221Byte7 = can2004Data[6];

    _canMessageHandlerContainer2010->SetData(0x221);
    _canMessageHandlerContainer2010->SendMessageForced(0x221, _currentTime);
}

void IRAM_ATTR CanDataConverter::Handle_1A1()
{
    _dataBroker->S_1A1Byte1 = can2004Data[0];
    _dataBroker->S_1A1Byte2 = can2004Data[1];
    _dataBroker->S_1A1Byte3 = can2004Data[2];
    _dataBroker->S_1A1Byte4 = can2004Data[3];
    _dataBroker->S_1A1Byte5 = can2004Data[4];
    _dataBroker->S_1A1Byte6 = can2004Data[5];
    _dataBroker->S_1A1Byte7 = can2004Data[6];
    _dataBroker->S_1A1Byte8 = can2004Data[7];

    _canMessageHandlerContainer2010->SetData(0x1A1);
    _canMessageHandlerContainer2010->SendMessageForced(0x1A1, _currentTime);
}

void IRAM_ATTR CanDataConverter::Handle_220()
{
    Can220_2004_Struct tmp;
    memcpy(&tmp, can2004Data, sizeof(tmp));
    _dataBroker->IsFrontLeftDoorOpen  = tmp.Field1.data.front_left_door_open;
    _dataBroker->IsFrontRightDoorOpen = tmp.Field1.data.front_right_door_open;
    _dataBroker->IsRearLeftDoorOpen   = tmp.Field1.data.rear_left_door_open;
    _dataBroker->IsRearRightDoorOpen  = tmp.Field1.data.rear_right_door_open;
    _dataBroker->IsBootLidOpen        = tmp.Field1.data.trunk_open;
    _dataBroker->IsHoodOpen           = tmp.Field1.data.hood_open;
    _dataBroker->IsFuelFlapOpen       = tmp.Field1.data.fuel_flap_open;
    _dataBroker->IsRearWindowOpen     = tmp.Field1.data.rear_window_open;
    _dataBroker->DoorStatusByte       = tmp.Field1.asByte;

    _canMessageHandlerContainer2010->SetData(0x1A1);
    _canMessageHandlerContainer2010->SendMessageForced(0x1A1, _currentTime);
}

void IRAM_ATTR CanDataConverter::Handle_127()
{
    Can127_2004_Struct tmp;
    memcpy(&tmp, can2004Data, sizeof(tmp));

    _dataBroker->EnableVTH = tmp.Status.data.enable_vth;

    _canMessageHandlerContainer2010->SetData(0x168);
}

void IRAM_ATTR CanDataConverter::Handle_21F()
{
    _dataBroker->RemoteCommand1 = can2004Data[0];
    _dataBroker->RemoteScrollPosition = can2004Data[1];

    if (_msgLength > 2)
    {
        _dataBroker->RemoteCommand3 = can2004Data[2];
    }

    _canMessageHandlerContainer2010->SetData(0x21F);
}