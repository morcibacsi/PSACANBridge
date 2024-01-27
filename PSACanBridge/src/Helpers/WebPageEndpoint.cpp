#include "WebPageEndpoint.h"

#ifdef WIFI_ENABLED
#include <ArduinoJson.h>
#include "WWWData.h"

WebServer* _webServer;

void handleRootPage() {
    _webServer->sendHeader("Content-Encoding", "gzip");
    const char* charArray = reinterpret_cast<const char*>(ESP_REACT_DATA_0);
    _webServer->send_P(200, "text/html", charArray, sizeof(ESP_REACT_DATA_0));
}

void handleData_1() {
    _webServer->sendHeader("Content-Encoding", "gzip");
    const char* charArray = reinterpret_cast<const char*>(ESP_REACT_DATA_1);
    _webServer->send_P(200, "text/html", charArray, sizeof(ESP_REACT_DATA_1));
}

void handleData_2() {
    _webServer->sendHeader("Content-Encoding", "gzip");
    const char* charArray = reinterpret_cast<const char*>(ESP_REACT_DATA_2);
    _webServer->send_P(200, "text/html", charArray, sizeof(ESP_REACT_DATA_2));
}

void handleData_3() {
    _webServer->sendHeader("Content-Encoding", "gzip");
    const char* charArray = reinterpret_cast<const char*>(ESP_REACT_DATA_3);
    _webServer->send_P(200, "text/html", charArray, sizeof(ESP_REACT_DATA_3));
}

void handleNotFound() {
    _webServer->send(404, "application/json", "{\"message\":\"Endpoint not found\"}");
}

WebPageEndpoint::WebPageEndpoint(WebServer* webServer)
{
    _webServer = webServer;

    _webServer->on("/generate_204", HTTP_GET, handleRootPage);
    _webServer->on("/", HTTP_GET, handleRootPage);
    _webServer->on("/index.html", HTTP_GET, handleRootPage);
    _webServer->on("/scripts/main.js", HTTP_GET, handleData_1);
    _webServer->on("/scripts/vendor.js", HTTP_GET, handleData_2);
    _webServer->on("/styles/app.css", HTTP_GET, handleData_3);
    _webServer->onNotFound(handleNotFound);
}
#endif