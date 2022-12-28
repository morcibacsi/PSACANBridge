#include "MessageHandler_168.h"
#include "../Structs/CAN_168_2010.h"

void MessageHandler_168::SetData()
{
    CanDash3Byte1_2010_Struct field1;
    field1.asByte = _dataBroker->S_168Byte1;
    //other bits are equal to CAN2004 version
    field1.data.number_of_gears = 0;

    CanDash3Byte2_2010_Struct field2;
    field2.data.max_rpm_2           = _dataBroker->EngineSpeedThreshold2;
    field2.data.max_rpm_1           = _dataBroker->EngineSpeedThreshold1;
    field2.data.auto_wiping_active  = _dataBroker->AutoWipingActive;
    field2.data.fap_clogged         = _dataBroker->FapClogged;
    field2.data.flat_tyre_alert     = _dataBroker->FlatTyreAlert;
    field2.data.tyre_pressure_alert = _dataBroker->TyrePressureAlert;

    CanDash3Byte3_2010_Struct field3;
    field3.data.generator_fault          = _dataBroker->GeneratorFault;
    field3.data.battery_charge_fault     = _dataBroker->BatteryChargeFault;
    field3.data.serious_suspension_fault = _dataBroker->SeriousSuspensionFault;
    field3.data.serious_ref_ehb_fault    = _dataBroker->SeriousREFFault;

    CanDash3Byte4_2010_Struct field4;
    field4.data.mil                  = _dataBroker->Mil;
    field4.data.brake_pad_fault      = _dataBroker->BrakePadFault;
    field4.data.gearbox_fault        = _dataBroker->AutoGearboxFault;
    field4.data.esp_fault            = _dataBroker->EspFault;
    field4.data.abs_fault            = _dataBroker->AbsFault;
    field4.data.fse_tightening_fault = _dataBroker->FSETighteningFault;
    field4.data.fse_system_fault     = _dataBroker->FSESystemFault;

    CanDash3Byte5_2010_Struct field5;
    field5.data.stt_lamp_status      = _dataBroker->STTLampStatus;
    field5.data.power_steering_fault = _dataBroker->PowerSteeringFault;
    field5.data.caar_lamp_status     = _dataBroker->CAARLampStatus;
    field5.data.curve_code_fault     = _dataBroker->CurveCodeFault;
    field5.data.water_in_diesel      = _dataBroker->WaterInDiesel;

    CanDash3Byte7_2010_Struct field7;
    field7.data.gearbox_position = _dataBroker->GearPosition;
    field7.data.authorize_vth    = _dataBroker->EnableVTH;

    _data[0] = field1.asByte;
    _data[1] = field2.asByte;
    _data[2] = field3.asByte;
    _data[3] = field4.asByte;
    _data[4] = field5.asByte;
    _data[6] = field7.asByte;

     _wasDataSet = true;
}

