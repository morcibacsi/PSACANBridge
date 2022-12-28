#include "MessageHandler_228.h"
#include "../Structs/CAN_228_2010.h"

void MessageHandler_228::SetData()
{
    CAN_228_2010_Byte3Struct byte2;
    byte2.data.activate_function           = _dataBroker->CruiseControlActivateFunction;
    byte2.data.selected_function           = _dataBroker->CruiseControlSelectedFunction;
    byte2.data.status_of_selected_function = _dataBroker->CruiseControlStatusOfSelectedFunction;
    byte2.data.setting_status = SETTING_STATUS_NO_ADJUSTMENT;
    byte2.data.target_present = 1;
    //byte2.data. = 1;

    _data[0] = _dataBroker->CruiseSpeed1;// CruiseControlSpeed.data.leftByte;
    _data[1] = _dataBroker->CruiseSpeed2; //CruiseControlSpeed.data.rightByte;
    //_data[2] = _dataBroker->CruiseByte0;// byte2.asByte;
    _data[2] = byte2.asByte;
    _data[3] = 0x80;
    _data[4] = 0x14;
    _data[5] = 0x7F;
    _data[6] = 0xFF;
    _data[7] = 0x98;

     _wasDataSet = true;
}
