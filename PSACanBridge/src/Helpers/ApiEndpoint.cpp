#include "ApiEndpoint.h"

#ifdef WIFI_ENABLED
#include <ArduinoJson.h>
#include "DebugPrint.h"
#include <Update.h>

static CanMessageHandlerContainer2010* _canMessageHandler;
static Config* _config;
static ConfigStorageEsp32 *_configStorage;
static TimeProvider *_timeProvider;
static DataBroker *_dataBroker;
static WebServer* _webServer;
char response[JSON_MEMORY_SIZE];

void sendResponse(bool success, int8_t failedAt, uint8_t trace)
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
    serializeJson(jsonResponse, response);
    _webServer->send(200, "application/json", response);
}

void handleRebootGetEvent()
{
    sendResponse(true, 0, 0);
    ESP.restart();
}

void handleKeepAliveGetEvent()
{
    _dataBroker->LastWebPageActivity = millis();
    sendResponse(true, 0, 0);
}

void handleConfigGetEvent()
{
    DynamicJsonDocument jsonResponse(JSON_MEMORY_SIZE);
    jsonResponse = _configStorage->GetConfig();
    // Print to response
    serializeJson(jsonResponse, response);
    _webServer->send(200, "application/json", response);
}

void handleConfigPostEvent()
{
    int requestCapacity = JSON_MEMORY_SIZE;

    String body = _webServer->arg("plain");

    uint8_t saveStatus = 0;
    int8_t failedAt = -1;
    uint8_t trace = 0;

    DynamicJsonDocument jsonRequest(JSON_MEMORY_SIZE);
    if (deserializeJson(jsonRequest, body) == DeserializationError::Ok)
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
        sendResponse(true, 0, 0);
    }
    else
    {
        sendResponse(false, failedAt, trace);
    }
}

void handleConfigDeleteEvent()
{
    _configStorage->Remove();
    sendResponse(true, 0, 0);
}

void handleDateTimePostEvent()
{
    uint16_t yearParam;
    uint8_t monthParam;
    uint8_t dayParam;
    uint8_t hourParam;
    uint8_t minuteParam;
    uint8_t secondParam;

    int requestCapacity = JSON_MEMORY_SIZE;

    String body = _webServer->arg("plain");

    uint8_t saveStatus = 0;
    int8_t failedAt = -1;
    uint8_t trace = 0;

    DynamicJsonDocument jsonRequest(JSON_MEMORY_SIZE);
    if (deserializeJson(jsonRequest, body) == DeserializationError::Ok)
    {
        yearParam = jsonRequest["year"];
        monthParam = jsonRequest["month"];
        dayParam = jsonRequest["day"];
        hourParam = jsonRequest["hour"];
        minuteParam = jsonRequest["minute"];
        secondParam = jsonRequest["second"];

        _timeProvider->Start();

        _timeProvider->SetDateTime(
            yearParam,
            monthParam,
            dayParam,
            hourParam,
            minuteParam,
            secondParam
        );

        saveStatus = 1;
    }
    else
    {
        failedAt = -2;
    }

    if (saveStatus == 1)
    {
        _canMessageHandler->SetData(0x260);
        sendResponse(true, 0, 0);
    }
    else
    {
        sendResponse(false, failedAt, trace);
    }

    sendResponse(true, 0, 0);
}
///*

static bool opened = false;
void handleOtaUpdate()
{
    HTTPUpload& upload = _webServer->upload();

    if(opened == false)
    {
        Serial.println("Update");
        // if filename includes spiffs, update the spiffs partition
        //int cmd = (filename.indexOf("spiffs") > -1) ? U_SPIFFS : U_FLASH;
        int cmd = U_SPIFFS;
        if (!Update.begin(UPDATE_SIZE_UNKNOWN, cmd)) {
            Update.printError(Serial);
        }
    }

    if(upload.status == UPLOAD_FILE_WRITE)
    {
        if(Update.write(upload.buf, upload.currentSize) != upload.currentSize){
            Update.printError(Serial);
            return;
        }
    }
    else if(upload.status == UPLOAD_FILE_END)
    {
        sendResponse(true, 0, 0);
        opened = false;

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
    WebServer* webServer,
    CanMessageHandlerContainer2010* canMessageHandler,
    Config *config,
    ConfigStorageEsp32 *configStorage,
    TimeProvider *timeProvider,
    DataBroker *dataBroker
    )
{
    _canMessageHandler = canMessageHandler;
    _config = config;
    _configStorage = configStorage;
    _timeProvider = timeProvider;
    _dataBroker = dataBroker;
    _webServer = webServer;

    _webServer->on("/api/reboot", HTTP_GET, handleRebootGetEvent);

    _webServer->on("/api/keepalive", HTTP_GET, handleKeepAliveGetEvent);

    _webServer->on("/api/config", HTTP_GET, handleConfigGetEvent);

    _webServer->on("/api/config", HTTP_DELETE, handleConfigDeleteEvent);

    _webServer->on("/api/datetime", HTTP_POST, handleDateTimePostEvent);
///*
    _webServer->on("/otaupdate", HTTP_POST, handleOtaUpdate);
//*/
    _webServer->on("/api/config", HTTP_POST, handleConfigPostEvent);
}
#endif