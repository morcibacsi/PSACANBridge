#include "ApiEndpoint.h"

#ifdef WIFI_ENABLED
#include <ArduinoJson.h>
#include "DebugPrint.h"
#include <Update.h>

static CanMessageHandlerContainer2010* _canMessageHandler;
static Config* _config;
static ConfigStorageEsp32 *_configStorage;
static TimeProvider *_timeProvider;

void sendResponse(AsyncWebServerRequest* request, bool success, int8_t failedAt, uint8_t trace)
{
    //int responseCapacity = 48;
    int responseCapacity = JSON_MEMORY_SIZE;
    DynamicJsonDocument jsonResponse(responseCapacity);
    if (success)
    {
        jsonResponse[F("success")] = F("OK");
    }
    else
    {
        jsonResponse[F("success")] = F("FAILED");
        jsonResponse[F("failedAt")] = failedAt;
        jsonResponse[F("trace")] = trace;
    }

    // Print to response
    AsyncResponseStream* response = request->beginResponseStream("application/json");
    serializeJson(jsonResponse, *response);
    request->send(response);
}

void handleConfigGetEvent(AsyncWebServerRequest* request)
{
    AsyncResponseStream* response = request->beginResponseStream("application/json");
    DynamicJsonDocument jsonResponse(JSON_MEMORY_SIZE);
    jsonResponse = _configStorage->GetConfig();
    // Print to response
    serializeJson(jsonResponse, *response);
    request->send(response);
}

void handleConfigPostEvent(AsyncWebServerRequest* request, uint8_t *data, size_t len)
{
    int requestCapacity = JSON_MEMORY_SIZE;

    // Create buffer to read request
    char * buffer = new char[requestCapacity];
    memset(buffer, 0, requestCapacity);

    // Try to read request into buffer
    for (size_t i = 0; i < len; i++)
    {
        buffer[i] = data[i];
    }

    uint8_t saveStatus = 0;
    int8_t failedAt = -1;
    uint8_t trace = 0;

    DynamicJsonDocument jsonRequest(JSON_MEMORY_SIZE);
    if (deserializeJson(jsonRequest, buffer) == DeserializationError::Ok)
    {
        if (jsonRequest["HAS_RTC"] == true)
        {
            _timeProvider->Start();
        }
        _configStorage->LoadFromDoc(jsonRequest);
        _configStorage->Save();
        saveStatus = 1;
    }
    else
    {
        failedAt = -2;
    }

    if (saveStatus == 1)
    {
        _canMessageHandler->SetData(0x260);
        sendResponse(request, true, 0, 0);
    }
    else
    {
        sendResponse(request, false, failedAt, trace);
    }
}

void handleConfigDeleteEvent(AsyncWebServerRequest* request)
{
    _configStorage->Remove();
    sendResponse(request, true, 0, 0);
}

void handleDateTimePostEvent(AsyncWebServerRequest* request)
{
    AsyncWebParameter* yearParam   = request->getParam("year", true);
    AsyncWebParameter* monthParam  = request->getParam("month", true);
    AsyncWebParameter* dayParam    = request->getParam("day", true);
    AsyncWebParameter* hourParam   = request->getParam("hour", true);
    AsyncWebParameter* minuteParam = request->getParam("minute", true);
    AsyncWebParameter* secondParam = request->getParam("second", true);
/*
    Serial.println(yearParam->value());
    Serial.println(monthParam->value());
    Serial.println(dayParam->value());
    Serial.println(hourParam->value());
    Serial.println(minuteParam->value());
    Serial.println(secondParam->value());
*/

    _timeProvider->Start();

    _timeProvider->SetDateTime(
        yearParam->value().toInt(),
        monthParam->value().toInt(),
        dayParam->value().toInt(),
        hourParam->value().toInt(),
        minuteParam->value().toInt(),
        secondParam->value().toInt()
    );

    sendResponse(request, true, 0, 0);
}
///*
void handleOtaUpdate(AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final)
{
    if (!index){
        Serial.println("Update");
        // if filename includes spiffs, update the spiffs partition
        int cmd = (filename.indexOf("spiffs") > -1) ? U_SPIFFS : U_FLASH;
        if (!Update.begin(UPDATE_SIZE_UNKNOWN, cmd)) {
            Update.printError(Serial);
        }
    }

    if (Update.write(data, len) != len) {
        Update.printError(Serial);
    }

    if (final) {
        //AsyncWebServerResponse *response = request->beginResponse(302, "text/plain", "Please wait while the device reboots");
        //response->addHeader("Refresh", "20");
        //response->addHeader("Location", "/");
        //request->send(response);
        sendResponse(request, true, 0, 0);

        if (!Update.end(true)){
            Update.printError(Serial);
        }
        else
        {
            Serial.println("Update complete");
            Serial.flush();
            ESP.restart();
        }
    }
}
//*/

ApiEndpoint::ApiEndpoint(
    AsyncWebServer* webServer,
    CanMessageHandlerContainer2010* canMessageHandler,
    Config *config,
    ConfigStorageEsp32 *configStorage,
    TimeProvider *timeProvider
    )
{
    _canMessageHandler = canMessageHandler;
    _config = config;
    _configStorage = configStorage;
    _timeProvider = timeProvider;

    webServer->on("/api/config", HTTP_GET, [](AsyncWebServerRequest* request) {
        handleConfigGetEvent(request);
    });

    webServer->on("/api/config", HTTP_DELETE, [](AsyncWebServerRequest* request) {
        handleConfigDeleteEvent(request);
    });

    webServer->on("/api/datetime", HTTP_POST, [](AsyncWebServerRequest* request) {
        handleDateTimePostEvent(request);
    });
///*
    webServer->on("/otaupdate", HTTP_POST,
        [](AsyncWebServerRequest *request) {},
        [](AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data,
        size_t len, bool final) {
            handleOtaUpdate(request, filename, index, data, len, final);
        }
    );
//*/
    webServer->onRequestBody([](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
        if (request->url() == "/api/config") {
            handleConfigPostEvent(request, data, len);
        }
    });
}
#endif