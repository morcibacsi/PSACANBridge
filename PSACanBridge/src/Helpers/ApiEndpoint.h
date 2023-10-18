#include "../../Config.h"

#ifdef WIFI_ENABLED

#ifndef ApiEndpoint_H
    #define ApiEndpoint_H

#include <inttypes.h>
#include "../Can/CanMessageHandlerContainer2010.h"
#include "../Helpers/ConfigStorageEsp32.h"
#include "../Helpers/TimeProvider.h"
#include "../Helpers/DataBroker.h"

// Includes for the server
#include <ESPAsyncWebServer.h>
#include <AsyncJson.h>

class ApiEndpoint
{
    private:
    public:
    ApiEndpoint(
        AsyncWebServer* webServer,
        CanMessageHandlerContainer2010* canMessageHandler,
        Config *config,
        ConfigStorageEsp32 *configStorage,
        TimeProvider *timeProvider,
        DataBroker *dataBroker
        );
    virtual ~ApiEndpoint(){ }
};
#endif
#endif