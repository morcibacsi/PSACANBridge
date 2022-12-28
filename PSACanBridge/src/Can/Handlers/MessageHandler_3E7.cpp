#include "MessageHandler_3E7.h"
#include "../Structs/CAN_3E7_2010.h"

void MessageHandler_3E7::SetData()
{
    CAN_3E7_2010Byte1Struct field1;
    field1.data.maintenance_type_km   = _dataBroker->IsMaintenanceDue;
    field1.data.wrench_icon           = _dataBroker->WrenchIcon;
    field1.data.maintenance_sign_km   = _dataBroker->MaintenanceSignKm;
    field1.data.maintenance_sign_time = _dataBroker->MaintenanceSignTime;
    field1.data.km_blinking           = _dataBroker->MaintenanceSignTimeBlinking || _dataBroker->MaintenanceSignKmBlinking;

    _data[0] = field1.asByte;
    _data[1] = _dataBroker->MaintenanceTime.data.leftByte;
    _data[2] = _dataBroker->MaintenanceTime.data.rightByte;
    _data[3] = _dataBroker->MaintenanceKilometers.data.leftByte;
    _data[4] = _dataBroker->MaintenanceKilometers.data.rightByte;

    _wasDataSet = true;
}
