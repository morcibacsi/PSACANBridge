#include "MessageHandler_276.h"
#include "../Structs/CAN_276_2010.h"

void MessageHandler_276::SetData()
{
    CAN_276_2010_Byte1Struct byte1;
    byte1.data.year = _dataBroker->Year - 2000;
    byte1.data.time_format = 1; // 24 hour format

    CAN_276_2010_Byte2Struct byte2;
    byte2.data.month = _dataBroker->Month;

    CAN_276_2010_Byte3Struct byte3;
    byte3.data.day = _dataBroker->MDay;

    CAN_276_2010_Byte4Struct byte4;
    byte4.data.hour = _dataBroker->Hour;

    CAN_276_2010_Byte5Struct byte5;
    byte5.data.minute = _dataBroker->Minute;

    _data[0] = byte1.asByte;
    _data[1] = byte2.asByte;
    _data[2] = byte3.asByte;
    _data[3] = byte4.asByte;
    _data[4] = byte5.asByte;;
    _data[5] = 0x00;
    _data[6] = 0x00;
    _data[7] = 0x00;
}
