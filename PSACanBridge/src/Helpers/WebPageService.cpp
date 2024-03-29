#include "WebPageService.h"

#ifdef WIFI_ENABLED
#include "../SerialPort/WebSocketSerialAbs.h"
#include "DebugPrint.h"
#include "../../Config.h"

#include <WiFi.h>
#include <DNSServer.h>

static bool _wifiStarted = false;
static bool _startInited = false;
static AbsSer* _serialPort;
static DataBroker *_dataBroker;

IPAddress apIP(192, 168, 100, 1);
const char *hosturl = "http://192.168.100.1";

void WiFiEvent(WiFiEvent_t event)
{
    if (event == ARDUINO_EVENT_WIFI_STA_START || event == ARDUINO_EVENT_WIFI_AP_START)
    {
        _wifiStarted = true;
        WiFi.setTxPower(WIFI_POWER_MINUS_1dBm);
    }

    if (event == ARDUINO_EVENT_WIFI_STA_GOT_IP || event == ARDUINO_EVENT_ETH_GOT_IP)
    {
        _serialPort->print("Obtained IP address: ");
        _serialPort->println(WiFi.localIP());
    }

    if (event == ARDUINO_EVENT_WIFI_STA_DISCONNECTED || event == ARDUINO_EVENT_ETH_DISCONNECTED ||
        event == ARDUINO_EVENT_WIFI_STA_STOP )
        //event == ARDUINO_EVENT_WIFI_STA_STOP || event == ARDUINO_EVENT_WIFI_AP_STOP)
    {
        //_wifiStarted = false;
        //_startInited = false;
        _serialPort->println("WiFi stopped");
    }

//
/*
    debug_print("[WiFi-event] event: ");
    debug_println(event);

    switch (event) {
        case ARDUINO_EVENT_WIFI_READY:
            debug_println("WiFi interface ready");
            break;
        case ARDUINO_EVENT_WIFI_SCAN_DONE:
            debug_println("Completed scan for access points");
            break;
        case ARDUINO_EVENT_WIFI_STA_START:
            debug_println("WiFi client started");
            break;
        case ARDUINO_EVENT_WIFI_STA_STOP:
            debug_println("WiFi clients stopped");
            break;
        case ARDUINO_EVENT_WIFI_STA_CONNECTED:
            debug_println("Connected to access point");
            break;
        case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
            debug_println("Disconnected from WiFi access point");
            break;
        case ARDUINO_EVENT_WIFI_STA_AUTHMODE_CHANGE:
            debug_println("Authentication mode of access point has changed");
            break;
        case ARDUINO_EVENT_WIFI_STA_GOT_IP:
            debug_print("Obtained IP address: ");
            debug_println(WiFi.localIP());
            break;
        case ARDUINO_EVENT_WIFI_STA_LOST_IP:
            debug_println("Lost IP address and IP address is reset to 0");
            break;
        case ARDUINO_EVENT_WPS_ER_SUCCESS:
            debug_println("WiFi Protected Setup (WPS): succeeded in enrollee mode");
            break;
        case ARDUINO_EVENT_WPS_ER_FAILED:
            debug_println("WiFi Protected Setup (WPS): failed in enrollee mode");
            break;
        case ARDUINO_EVENT_WPS_ER_TIMEOUT:
            debug_println("WiFi Protected Setup (WPS): timeout in enrollee mode");
            break;
        case ARDUINO_EVENT_WPS_ER_PIN:
            debug_println("WiFi Protected Setup (WPS): pin code in enrollee mode");
            break;
        case ARDUINO_EVENT_WPS_ER_PBC_OVERLAP:
            debug_println("ESP32 station wps overlap in enrollee mode");
            break;
        case ARDUINO_EVENT_WIFI_AP_START:
            debug_println("WiFi access point started");
            break;
        case ARDUINO_EVENT_WIFI_AP_STOP:
            debug_println("WiFi access point  stopped");
            break;
        case ARDUINO_EVENT_WIFI_AP_STACONNECTED:
            debug_println("Client connected");
            break;
        case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED:
            debug_println("Client disconnected");
            break;
        case ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED:
            debug_println("Assigned IP address to client");
            break;
        case ARDUINO_EVENT_WIFI_AP_PROBEREQRECVED:
            debug_println("Received probe request");
            break;
        case ARDUINO_EVENT_WIFI_AP_GOT_IP6:
            debug_println("AP IPv6 is preferred");
            break;
        case ARDUINO_EVENT_ETH_START:
            debug_println("Ethernet started");
            break;
        case ARDUINO_EVENT_ETH_STOP:
            debug_println("Ethernet stopped");
            break;
        case ARDUINO_EVENT_ETH_CONNECTED:
            debug_println("Ethernet connected");
            break;
        case ARDUINO_EVENT_ETH_DISCONNECTED:
            debug_println("Ethernet disconnected");
            break;
        case ARDUINO_EVENT_ETH_GOT_IP:
            debug_println("Obtained IP address");
            break;
        default: break;
    }
//*/
}

WebPageService::WebPageService(
    CanMessageHandlerContainer2010* canMessageHandler,
    Config *config,
    ConfigStorageEsp32 *configStorage,
    AbsSer *serialPort,
    TimeProvider *timeProvider,
    DataBroker *dataBroker
    )
{
    _config = config;
    _configStorage = configStorage;
    _timeProvider = timeProvider;
    _canMessageHandler = canMessageHandler;
    _dataBroker = dataBroker;
    _serialPort = serialPort;

    WiFi.onEvent(WiFiEvent);
}

void WebPageService::StartWifiAP()
{
    WiFi.mode(WIFI_MODE_AP);
    WiFi.softAP(_config->WIFI_AP_NAME, _config->WIFI_PASSWORD);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
    dnsServer.start(53, "*", apIP);
}

void WebPageService::StartWifiClient()
{
    WiFi.begin(_config->WIFI_SSID, _config->WIFI_PASSWORD);
}

void WebPageService::Start()
{
    if (_startInited)
    {
        return;
    }
    _startInited = true;
    _stopInitiated = false;
    if (_config->WIFI_AP_MODE)
    {
        StartWifiAP();
    }
    else
    {
        StartWifiClient();
    }
}

void WebPageService::Stop()
{
    WiFi.disconnect(true);  // Disconnect from the network
    WiFi.mode(WIFI_OFF);    // Switch WiFi off
    _startInited = false;
    _wifiStarted = false;
}

bool WebPageService::IsRunning()
{
    return _wifiStarted || _startInited;
}

void WebPageService::Loop(unsigned long currentTime)
{
    /*
    if (_config->WIFI_TIMEOUT > 0 && _wifiStarted && (currentTime - _dataBroker->LastWebPageActivity) > _config->WIFI_TIMEOUT && !_stopInitiated)
    {
        _stopInitiated = true;
        Stop();
    }
    */

    if (_wifiStarted && !_serverStarted)
    {
        //debug_println(WiFi.localIP());
        webServer = new WebServer(80);

        webPageEndpoint = new WebPageEndpoint(webServer);
        apiEndpoint = new ApiEndpoint(webServer, _canMessageHandler, _config, _configStorage, _timeProvider, _dataBroker);
        webServer->begin();
        _serverStarted = true;
        _dataBroker->LastWebPageActivity = currentTime;

        debug_println("WebPageService started");
    }

    if (_serverStarted && _wifiStarted)
    {
        //debug_println("handleClient");
        webServer->handleClient();
        if (_config->WIFI_AP_MODE){
            dnsServer.processNextRequest();
        }
    }
}

WebServer* WebPageService::GetHTTPServer()
{
    return webServer;
}
#endif