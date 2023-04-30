#include "MessageHandler_21F.h"
#include "../Structs/CAN_21F.h"
#include "../Structs/CAN_21F_2010.h"

void MessageHandler_21F::SetData()
{
    _data[0] = _dataBroker->RemoteCommand1;
    _data[1] = _dataBroker->RemoteScrollPosition;
    _data[2] = _dataBroker->RemoteCommand3;

    if (_config->REPLACE_REMOTE_MODE_BTN_WITH_SRC)
    {
        CAN_21F_Byte1Struct field1;
        field1.asByte = _dataBroker->RemoteCommand1;

        uint8_t modePushed = field1.data.mode_phone;
        field1.data.mode_phone = 0;
        _data[0] = field1.asByte;

        CAN_21F_2010_Byte3Struct field3;
        field3.asByte = _dataBroker->RemoteCommand3;
        field3.data.source = modePushed;
        _data[2] = field3.asByte;
    }

     _wasDataSet = true;
}
