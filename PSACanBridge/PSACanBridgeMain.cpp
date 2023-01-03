#include <Arduino.h>
#include <SPI.h>
#include "Config.h"
#include "src/Can/CanMessageSenderEsp32Idf.h"
#include "src/Can/CanMessageSenderMCP2515.h"
#include "src/Can/CanMessageSenderOnSerial.h"
#include "src/Can/CanDataConverter.h"
#include "src/Can/ICanMessageSender.h"
#include "src/Can/Structs/CAN_0E1.h"

#include "src/Can/CanMessageHandlerContainer2010.h"

#include "src/Helpers/IGetDeviceInfo.h"
#include "src/Helpers/GetDeviceInfoEsp32.h"
#include "src/Helpers/SerialReader.h"
#include "src/Helpers/DataBroker.h"
#include "src/Helpers/IntUnions.h"
#include "src/Helpers/ConfigStorageEsp32.h"
#include "src/Helpers/DebugPrint.h"
#include "src/Helpers/MessageDecider.h"
#include "src/Helpers/TimeProvider.h"

#ifdef USE_BLUETOOTH_SERIAL
    #include <BluetoothSerial.h>
    #include "src/SerialPort/BluetoothSerialAbs.h"
    BluetoothSerial SerialBT;
#else
    #include "src/SerialPort/HardwareSerialAbs.h"
#endif

#ifdef WIFI_ENABLED
    #ifdef WEBSOCKET_SERIAL
        #include "src/SerialPort/WebSocketSerialAbs.h"
    #endif
    #include "src/Helpers/WebPageService.h"
    WebPageService* webPageService;
    TaskHandle_t RunWebPageTask;
#endif

// CAN 2010
constexpr uint8_t CAN_RX_PIN = 18;
constexpr uint8_t CAN_TX_PIN = 15;

SPIClass* spi2004;

AbsSer *serialPort;
IGetDeviceInfo *deviceInfo;
ICanMessageSender *can2010Interface;
ICanMessageSender *can2004Interface;
CanDataConverter *canDataConverter;
DataBroker *dataBroker;
Config *config;
MessageDecider *messageDecider;
TimeProvider *timeProvider;

TaskHandle_t CAN2010WriteTask;
TaskHandle_t CAN2004ReadDataTask;
TaskHandle_t CAN2004WriteTask;
TaskHandle_t CAN2010ReadTask;

CanMessageHandlerContainer2010 *canMessageHandlerContainer2010;
ConfigStorageEsp32 *configStorage;

void PrintCANArrayToSerial(const uint16_t canId, const uint8_t canMessage[], uint8_t canLength)
{
    char tmp[3];

    if (canId > 0)
    {
        //if (!(canId == 0x760 || canId == 0x660))
        {
            //return;
        }
        serialPort->print(canId, HEX);
        serialPort->print(",1,");
        serialPort->print(canLength, DEC);
        serialPort->print(",");

        for (size_t i = 0; i < canLength; i++)
        {
            snprintf(tmp, 3, "%02X", canMessage[i]);
            if (i != canLength - 1)
            {
                serialPort->print(tmp);
                serialPort->print("");
            }
        }
        serialPort->println(tmp);
    }
}

void CAN2010ReadTaskFunction(void * parameter)
{
    uint16_t canId;
    uint8_t canLength;
    uint8_t canMessage[8];
    unsigned long currentTime;

    for (;;)
    {
        canId = 0;
        can2010Interface->ReadMessage(&canId, &canLength, canMessage);
        currentTime = millis();

        if (canId > 0)
        {
            //PrintCANArrayToSerial(canId, canMessage, canLength);
            /*
            if (canId == 0x217)
            {
            }

            if (canId == 0x167)
            {
            }

            if (canId == 0x257)
            {
                debug_print("Odo km:");
                debug_print(canMessage[0], HEX);
                debug_print(canMessage[1], HEX);
                debug_print(canMessage[2], HEX);
                debug_println(canMessage[3], HEX);
            }
            */
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void IRAM_ATTR CAN2004ReadTaskFunction(void * parameter)
{
    unsigned long currentTime = 0;
    uint16_t canId = 0;
    uint8_t canLength = 0;
    uint8_t canMessage[8] = {0};

    SerialReader *serialReader = new SerialReader(serialPort, config, dataBroker);

    if (!config->REPLAY_MODE)
    {
    }

    for (;;)
    {
        currentTime = millis();
        canId = 0;
        canLength = 0;

        //serialReader->Receive(&canLength, canMessage);
        if (config->REPLAY_MODE == false)
        {
            //serialPort->println("Read 2004");
            can2004Interface->ReadMessage(&canId, &canLength, canMessage);
        }

        if (canId > 0)
        {
            //serialPort->println("Read 2004");
            if (!config->REPLAY_MODE && dataBroker->IsCan2004LogEnabled)
            {
                PrintCANArrayToSerial(canId, canMessage, canLength);
            }

            uint8_t shouldProcess = messageDecider->ShouldProcess(canId);
            if (shouldProcess == SHOULD_MODIFY_FOR_2010)
            {
                canDataConverter->ProcessMessage2004(currentTime, canId, canMessage, canLength);
                dataBroker->ProcessedPackets++;
            }
            if (shouldProcess == SHOULD_FORWARD_TO_2010)
            {
                can2010Interface->SendMessage(canId, 0, canLength, canMessage);
            }
        }
        if (config->REPLAY_MODE)
        {
            vTaskDelay(15 / portTICK_PERIOD_MS);
        }
    }
}

void CAN2010WriteTaskFunction(void * parameter)
{
    unsigned long currentTime = 0;

    for (;;)
    {
        currentTime = millis();

        timeProvider->Process(currentTime);
        canMessageHandlerContainer2010->SendDueMessages(currentTime);
        canDataConverter->SendCanMessage(currentTime);

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void CAN2004WriteTaskFunction(void* parameter)
{
    unsigned long currentTime = 0;
    for (;;)
    {
        currentTime = millis();

        //serialPort->println("Ignition");
        //uint8_t data1[] = { 0x0E, 0x00, 0x05, 0x2F, 0x21, 0x80, 0x00, 0xA0 };
        //uint8_t data2[] = { 0x08, 0x32, 0x00, 0x1F, 0x00, 0x0D, 0x40, 0x01 };

        //can2004Interface->SendMessage(0x036, 0, 8, data1);
        //can2004Interface->SendMessage(0x0F6, 0, 8, data2);

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

#ifdef WIFI_ENABLED
void RunWebPageTaskFunction(void* parameter)
{
    for (;;)
    {
        webPageService->Loop();

        vTaskDelay(15 / portTICK_PERIOD_MS);
    }
}
#endif

void InitSerialPort()
{
    uint16_t uniqueIdForBluetooth = 0;
    uniqueIdForBluetooth = deviceInfo->GetId();

    char bluetoothDeviceName[27];
    snprintf(bluetoothDeviceName, 27, "CAN2004 to CAN2010 Bridge %04X", uniqueIdForBluetooth);

#ifdef USE_BLUETOOTH_SERIAL
    serialPort = new BluetoothSerAbs(SerialBT, bluetoothDeviceName);
#else
    serialPort = new HwSerAbs(Serial);
#endif

    serialPort->begin(500000);
    serialPort->println(bluetoothDeviceName);
    serialPort->print("ESP version:");
    serialPort->println(esp_get_idf_version());
}

void Init2004()
{
    constexpr uint8_t SCK_PIN  = 25;
    constexpr uint8_t MISO_PIN = 5;
    constexpr uint8_t MOSI_PIN = 33;
    constexpr uint8_t CS_PIN   = 32;

    spi2004 = new SPIClass();
    spi2004->begin(SCK_PIN, MISO_PIN, MOSI_PIN, CS_PIN);
    can2004Interface = new CanMessageSenderMCP2515(CS_PIN, CAN_125KBPS, MCP_8MHZ, spi2004);
}

void Init2010()
{
    can2010Interface = new CanMessageSenderEsp32Idf(CAN_RX_PIN, CAN_TX_PIN, false, serialPort);
    //canInterface = new CanMessageSenderOnSerial(serialPort);
    can2010Interface->Init();
}

void setup()
{
    constexpr uint8_t SDA_PIN = 25;
    constexpr uint8_t SCL_PIN = 5;

    deviceInfo = new GetDeviceInfoEsp32();
    dataBroker = new DataBroker();
    config = new Config();
    timeProvider = new TimeProvider(SDA_PIN, SCL_PIN, dataBroker, config);
    timeProvider->Start();

    InitSerialPort();

    Init2010();
    Init2004();

    configStorage = new ConfigStorageEsp32(config);
    configStorage->Load();

    canMessageHandlerContainer2010 = new CanMessageHandlerContainer2010(can2010Interface, config, dataBroker);

    messageDecider = new MessageDecider();

    canDataConverter = new CanDataConverter(
        can2010Interface,
        config,
        dataBroker,
        serialPort,
        canMessageHandlerContainer2010
    );

    #ifdef WIFI_ENABLED
        webPageService = new WebPageService(canMessageHandlerContainer2010, config, configStorage, serialPort, timeProvider);

        #ifdef WEBSOCKET_SERIAL
        serialPort = new WebSocketSerAbs(webPageService->GetHTTPServer(), "/log");
        #endif

        webPageService->Start();
    #endif

    xTaskCreatePinnedToCore(
        CAN2010WriteTaskFunction,       // Function to implement the task
        "CAN2010WriteTask",             // Name of the task
        15000,                          // Stack size in words
        NULL,                           // Task input parameter
        2,                              // Priority of the task (higher the number, higher the priority)
        &CAN2010WriteTask,              // Task handle.
        0);                             // Core where the task should run
///*
    xTaskCreatePinnedToCore(
        CAN2004ReadTaskFunction,        // Function to implement the task
        "CAN2004ReadDataTask",          // Name of the task
        20000,                          // Stack size in words
        NULL,                           // Task input parameter
        1,                              // Priority of the task (higher the number, higher the priority)
        &CAN2004ReadDataTask,           // Task handle.
        1);                             // Core where the task should run
//*/
    xTaskCreatePinnedToCore(
        CAN2004WriteTaskFunction,       // Function to implement the task
        "CAN2004WriteTask",             // Name of the task
        20000,                          // Stack size in words
        NULL,                           // Task input parameter
        0,                              // Priority of the task (higher the number, higher the priority)
        &CAN2004WriteTask,              // Task handle.
        0);                             // Core where the task should run

    xTaskCreatePinnedToCore(
        CAN2010ReadTaskFunction,        // Function to implement the task
        "CAN2010ReadTask",              // Name of the task
        10000,                          // Stack size in words
        NULL,                           // Task input parameter
        1,                              // Priority of the task (higher the number, higher the priority)
        &CAN2010ReadTask,               // Task handle.
        0);                             // Core where the task should run

#ifdef WIFI_ENABLED
    xTaskCreatePinnedToCore(
        RunWebPageTaskFunction,         // Function to implement the task
        "RunWebPageTask",               // Name of the task
        10000,                          // Stack size in words
        NULL,                           // Task input parameter
        0,                              // Priority of the task (higher the number, higher the priority)
        &RunWebPageTask,                // Task handle.
        0);                             // Core where the task should run
#endif
}

void loop()
{
    vTaskDelay(50 / portTICK_PERIOD_MS);
    //esp_task_wdt_reset();
}
