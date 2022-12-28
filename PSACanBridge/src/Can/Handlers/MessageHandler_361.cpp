#include "MessageHandler_361.h"
#include "../Structs/CAN_361_2010.h"

void MessageHandler_361::SetData()
{
    Can361_2010Byte1Struct field1;
    field1.data.drl_present              = _dataBroker->DrlPresent;
    field1.data.auto_lighting            = _dataBroker->AutoLighting;
    field1.data.ambient_lighting         = _dataBroker->AmbientLighting;
    field1.data.blindspot_monitoring     = _dataBroker->BlindspotMonitoring;
    field1.data.highway_lighting_present = _dataBroker->HighwayLightingPresent;
    field1.data.setting_menu_availbale   = 1;

    Can361_2010Byte2Struct field2;
    field2.data.hinge_panel_select       = _dataBroker->HingePanelSelect;
    field2.data.permanent_rear_flap_lock = _dataBroker->PermanentRearFlapLock;
    field2.data.follow_me_home           = _dataBroker->FollowMeHome;
    field2.data.rear_wiper_option        = _dataBroker->RearWiperOption;
    field2.data.aas_disable              = _dataBroker->AasDisable;

    Can361_2010Byte3Struct field3;
    field3.data.automatic_electric_brake = _dataBroker->AutomaticElectricBrake;
    field3.data.config_enabled           = _dataBroker->ConfigEnabled;
    field3.data.tnb_present              = _dataBroker->TnbPresent;

    Can361_2010Byte4Struct field4;
    field4.data.breaking_on_alarm_risk = _dataBroker->BreakingOnAlarmRisk;
    field4.data.irc_present            = _dataBroker->IrcPresent;
    field4.data.tpms_present           = _dataBroker->TpmsPresent;
    field4.data.tpms_reset_present     = _dataBroker->TpmsResetPresent;

    _data[0] = field1.asByte;
    _data[1] = field2.asByte;
    _data[2] = field3.asByte;
    _data[3] = field4.asByte;

     _wasDataSet = true;
}
