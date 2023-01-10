#include "TimeProvider.h"

TimeProvider::TimeProvider(uint8_t sdaPin, uint8_t sclPin, DataBroker *dataBroker, Config *config)
{
    _dataBroker = dataBroker;
    _config = config;
    _sdaPin = sdaPin;
    _sclPin = sclPin;
}

void TimeProvider::Start()
{
    if (!_config->HAS_RTC || _started)
    {
        return;
    }

    // Initialize I2C
    Wire.begin(_sdaPin, _sclPin);
    Wire.setClock(100000);

    if (_rtc.begin())
    {
        _rtc.setSquareWave(SquareWaveDisable);
    }
    _started = true;
}

bool TimeProvider::Process(unsigned long currentTime)
{
    if (!_config->HAS_RTC)
    {
        return false;
    }

    uint8_t wday;
    if (currentTime - _previousTime >= 1000)
    {
        _previousTime = currentTime;
        _rtc.getDateTime(&_dataBroker->Hour, &_dataBroker->Minute, &_dataBroker->Second, &_dataBroker->MDay, &_dataBroker->Month, &_dataBroker->Year, &wday);

        return true;
    }

    return false;
}

void TimeProvider::SetDateTime(uint16_t year,  uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second)
{
    if (!_config->HAS_RTC)
    {
        return;
    }
    _rtc.setDateTime(hour, minute, second, day, month, year, 0);
}
