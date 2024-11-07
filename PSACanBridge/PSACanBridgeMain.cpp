#include <Arduino.h>
#include <SPI.h>
#include "Config.h"
#include "src/Can/CanMessageSenderEsp32Idf.h"
#include "src/Can/CanMessageSenderMCP2515CoryJ.h"
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
#include "src/Helpers/CPUConfig.h"

#ifdef BOARD_TYPE_ESP32_C3
    #include "BoardConfig_ESP32_C3.h"
#endif
#ifdef BOARD_TYPE_ESP32
    #include "BoardConfig_ESP32.h"
#endif
#ifdef BOARD_TYPE_ESP32_V15
    #include "BoardConfig_ESP32_v15.h"
#endif
#ifdef PIO_INI_BOARD_ESP32_C6
    #include "BoardConfig_ESP32_v16.h"
#endif

#ifdef USE_BLUETOOTH_SERIAL
    #include <BluetoothSerial.h>
    #include "src/SerialPort/BluetoothSerialAbs.h"
    BluetoothSerial SerialBT;
#else
    #include "src/SerialPort/HardwareSerialAbs.h"
    #include "src/SerialPort/USBSerialAbs.h"
#endif

#ifdef WIFI_ENABLED
    #ifdef WEBSOCKET_SERIAL
        #include "src/SerialPort/WebSocketSerialAbs.h"
    #endif
    #include "src/Helpers/WebPageService.h"
    WebPageService* webPageService;
    TaskHandle_t RunWebPageTask;
#endif


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
        if (webPageService->IsRunning())
        {
            vTaskDelay(10 / portTICK_PERIOD_MS);
            continue;
        }

        canId = 0;
        can2010Interface->ReadMessage(&canId, &canLength, canMessage);
        currentTime = millis();

        if (canId > 0)
        {
            //can2004Interface->SendMessage(canId, 0, canLength, canMessage);
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
    unsigned long lastSuccessfulRead = 0;
    unsigned long currentTime = 0;
    unsigned long ledTime = 0;
    uint16_t canId = 0;
    uint8_t canLength = 0;
    uint8_t canMessage[8] = {0};

    uint8_t ledPin = BOARD_LED_PIN;
    bool ledStatus = false;
    pinMode(ledPin, OUTPUT);

    SerialReader *serialReader = new SerialReader(serialPort, config, dataBroker);

    if (!config->REPLAY_MODE)
    {
    }

    for (;;)
    {
        if (webPageService->IsRunning())
        {
            vTaskDelay(10 / portTICK_PERIOD_MS);
            continue;
        }

        currentTime = millis();
        canId = 0;
        canLength = 0;

        if (currentTime - ledTime > 200)
        {
            ledTime = currentTime;
            digitalWrite(ledPin, ledStatus);
            ledStatus = !ledStatus;
        }

        //serialReader->Receive(&canLength, canMessage);
        if (config->REPLAY_MODE == false)
        {
            //serialPort->println("Read 2004");
            bool readSuccess = can2004Interface->ReadMessage(&canId, &canLength, canMessage);
            if (readSuccess)
            {
                lastSuccessfulRead = currentTime;
            }
            else if (currentTime - lastSuccessfulRead > 150)
            {
                serialPort->println("Resetting CAN2004");
                can2004Interface->Reset();
                //lastSuccessfulRead = 0;
                continue;
            }
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

            if (dataBroker->IsFrontLeftDoorOpen && dataBroker->LeftTurnIndicator == 1 && dataBroker->RightTurnIndicator == 1 && dataBroker->HighBeam == 1)
            {
                webPageService->Start();
                dataBroker->LastWebPageActivity = currentTime;
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

        if (timeProvider->Process(currentTime))
        {
            canMessageHandlerContainer2010->SetData(0x276);
        }

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
        if (webPageService->IsRunning())
        {
            vTaskDelay(10 / portTICK_PERIOD_MS);
            continue;
        }

        currentTime = millis();

        //serialPort->println("Ignition");
        //
        /*
        uint8_t data1[] = { 0x0E, 0x00, 0x05, 0x2F, 0x21, 0x80, 0x00, 0xA0 };
        uint8_t data2[] = { 0x08, 0x32, 0x00, 0x1F, 0x00, 0x0D, 0x40, 0x01 };

        can2004Interface->SendMessage(0x036, 0, 8, data1);
        can2004Interface->SendMessage(0x0F6, 0, 8, data2);
        //*/

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

#ifdef WIFI_ENABLED
void RunWebPageTaskFunction(void* parameter)
{
    unsigned long currentTime = 0;
    for (;;)
    {
        currentTime = millis();
        if (webPageService->IsRunning())
        {
            webPageService->Loop(currentTime);
        }
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
    #ifdef BOARD_TYPE_ESP32_C3
        serialPort = new UsbSerAbs(Serial);
    #else
        serialPort = new HwSerAbs(Serial);
    #endif
#endif

    serialPort->begin(500000);
    serialPort->println(bluetoothDeviceName);
    serialPort->print("ESP version:");
    serialPort->println(esp_get_idf_version());
}

void Init2004()
{
    #ifdef PIO_INI_BOARD_ESP32_C6
        uint8_t CAN_RX_PIN = BOARD_CAN_2004_RX_PIN;
        uint8_t CAN_TX_PIN = BOARD_CAN_2004_TX_PIN;

        can2004Interface = new CanMessageSenderEsp32Idf(CAN_RX_PIN, CAN_TX_PIN, false, serialPort, CanMessageSenderEsp32Idf::CAN_CONTROLLER_1);
    #else
        uint8_t SPI_INSTANCE = BOARD_SPI_INSTANCE;
        uint8_t SCK_PIN  = BOARD_SCK_PIN;
        uint8_t MISO_PIN = BOARD_MISO_PIN;
        uint8_t MOSI_PIN = BOARD_MOSI_PIN;
        uint8_t CS_PIN   = BOARD_CS_PIN;

        spi2004 = new SPIClass(SPI_INSTANCE);
        spi2004->begin(SCK_PIN, MISO_PIN, MOSI_PIN, CS_PIN);

        can2004Interface = new CanMessageSenderMCP2515CoryJ(CS_PIN, CAN_125KBPS, MCP_8MHZ, spi2004, serialPort);
    #endif

    can2004Interface->Init();
}

void Init2010()
{
    uint8_t CAN_RX_PIN = BOARD_CAN_RX_PIN;
    uint8_t CAN_TX_PIN = BOARD_CAN_TX_PIN;

    can2010Interface = new CanMessageSenderEsp32Idf(CAN_RX_PIN, CAN_TX_PIN, false, serialPort, CanMessageSenderEsp32Idf::CAN_CONTROLLER_0);
    //canInterface = new CanMessageSenderOnSerial(serialPort);
    can2010Interface->Init();
}

void setup()
{
    uint8_t SDA_PIN = BOARD_SDA_PIN;
    uint8_t SCL_PIN = BOARD_SCL_PIN;

    deviceInfo = new GetDeviceInfoEsp32();
    dataBroker = new DataBroker();
    config = new Config();

    InitSerialPort();

    Init2010();
    Init2004();

    configStorage = new ConfigStorageEsp32(config);
    //configStorage->Remove();
    bool configLoaded = configStorage->Load();

    timeProvider = new TimeProvider(SDA_PIN, SCL_PIN, dataBroker, config, serialPort);
    timeProvider->Start();

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
        webPageService = new WebPageService(canMessageHandlerContainer2010, config, configStorage, serialPort, timeProvider, dataBroker);

        #ifdef WEBSOCKET_SERIAL
        serialPort = new WebSocketSerAbs(webPageService->GetHTTPServer(), "/log");
        #endif

        if (configLoaded == false)
        {
            webPageService->Start();
        }
    #endif

    cpu_config_t CAN2010WriteTaskConfig    = { .cpu_core = 0, .priority = 2, .stack_size = 15000 };
    cpu_config_t CAN2004ReadDataTaskConfig = { .cpu_core = 1, .priority = 1, .stack_size = 20000 };
    cpu_config_t CAN2004WriteTaskConfig    = { .cpu_core = 0, .priority = 0, .stack_size = 20000 };
    cpu_config_t CAN2010ReadTaskConfig     = { .cpu_core = 0, .priority = 1, .stack_size = 10000 };
    cpu_config_t RunWebPageTaskConfig      = { .cpu_core = 0, .priority = 0, .stack_size = 10000 };

    if (CONFIG_SOC_CPU_CORES_NUM == 1)
    {
        CAN2010WriteTaskConfig    = { .cpu_core = 0, .priority = 5, .stack_size = 10000 };
        CAN2004ReadDataTaskConfig = { .cpu_core = 0, .priority = 4, .stack_size = 10000 };
        CAN2004WriteTaskConfig    = { .cpu_core = 0, .priority = 3, .stack_size = 10000 };
        CAN2010ReadTaskConfig     = { .cpu_core = 0, .priority = 2, .stack_size = 10000 };
        RunWebPageTaskConfig      = { .cpu_core = 0, .priority = 1, .stack_size = 10000 };
    }

    xTaskCreatePinnedToCore(
        CAN2010WriteTaskFunction,         // Function to implement the task
        "CAN2010WriteTask",               // Name of the task
        CAN2010WriteTaskConfig.stack_size,// Stack size in words
        NULL,                             // Task input parameter
        CAN2010WriteTaskConfig.priority,  // Priority of the task (higher the number, higher the priority)
        &CAN2010WriteTask,                // Task handle.
        CAN2010WriteTaskConfig.cpu_core); // Core where the task should run
///*
    xTaskCreatePinnedToCore(
        CAN2004ReadTaskFunction,              // Function to implement the task
        "CAN2004ReadDataTask",                // Name of the task
        CAN2004ReadDataTaskConfig.stack_size, // Stack size in words
        NULL,                                 // Task input parameter
        CAN2004ReadDataTaskConfig.priority,   // Priority of the task (higher the number, higher the priority)
        &CAN2004ReadDataTask,                 // Task handle.
        CAN2004ReadDataTaskConfig.cpu_core);  // Core where the task should run
//*/
    xTaskCreatePinnedToCore(
        CAN2004WriteTaskFunction,        // Function to implement the task
        "CAN2004WriteTask",              // Name of the task
        CAN2004WriteTaskConfig.stack_size,// Stack size in words
        NULL,                            // Task input parameter
        CAN2004WriteTaskConfig.priority, // Priority of the task (higher the number, higher the priority)
        &CAN2004WriteTask,               // Task handle.
        CAN2004WriteTaskConfig.cpu_core); // Core where the task should run

    xTaskCreatePinnedToCore(
        CAN2010ReadTaskFunction,         // Function to implement the task
        "CAN2010ReadTask",               // Name of the task
        CAN2010ReadTaskConfig.stack_size,// Stack size in words
        NULL,                            // Task input parameter
        CAN2010ReadTaskConfig.priority,  // Priority of the task (higher the number, higher the priority)
        &CAN2010ReadTask,                // Task handle.
        CAN2010ReadTaskConfig.cpu_core); // Core where the task should run

#ifdef WIFI_ENABLED
    xTaskCreatePinnedToCore(
        RunWebPageTaskFunction,         // Function to implement the task
        "RunWebPageTask",               // Name of the task
        RunWebPageTaskConfig.stack_size,// Stack size in words
        NULL,                           // Task input parameter
        RunWebPageTaskConfig.priority,  // Priority of the task (higher the number, higher the priority)
        &RunWebPageTask,                // Task handle.
        RunWebPageTaskConfig.cpu_core); // Core where the task should run
#endif
}

void loop()
{
    vTaskDelay(50 / portTICK_PERIOD_MS);
    //esp_task_wdt_reset();
}
