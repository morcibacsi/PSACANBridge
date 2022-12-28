#include "../../Config.h"

#ifdef WIFI_ENABLED

#ifndef WebPageService_H
    #define WebPageService_H

#include <inttypes.h>

#include "ApiEndpoint.h"
#include "WebPageEndpoint.h"
#include "../Can/CanMessageHandlerContainer2010.h"
#include "../Helpers/ConfigStorageEsp32.h"
#include "../Helpers/TimeProvider.h"
#include "../SerialPort/AbstractSerial.h"

// Includes for the server
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>

class WebPageService
{
    private:
    bool _serverStarted = false;
    CanMessageHandlerContainer2010* _canMessageHandler;
    Config* _config;
    ConfigStorageEsp32* _configStorage;
    TimeProvider *_timeProvider;

    DNSServer dnsServer;
    AsyncWebServer* webServer;
    ApiEndpoint* apiEndpoint;
    WebPageEndpoint* webPageEndpoint;

    void StartWifiAP();
    void StartWifiClient();

    public:
    WebPageService(
        CanMessageHandlerContainer2010* canMessageHandler,
        Config *config,
        ConfigStorageEsp32 *configStorage,
        AbsSer *serialPort,
        TimeProvider *timeProvider
        );
    virtual ~WebPageService(){ }
    void Start();
    AsyncWebServer* GetHTTPServer();
    void Loop();
};
#endif
#endif