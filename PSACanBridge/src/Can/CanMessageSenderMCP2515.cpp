#include "CanMessageSenderMCP2515.h"
//#include "Arduino.h"

CanMessageSenderMCP2515::CanMessageSenderMCP2515(uint8_t csPin, CAN_SPEED canSpeed, CAN_CLOCK canClock, SPIClass *spi)
{
    _csPin = csPin;

    can = new MCP2515(_csPin, spi);
    can->reset();
    can->setBitrate(canSpeed, canClock);
    can->setNormalMode();

    //canSemaphore = xSemaphoreCreateMutex();
}

void CanMessageSenderMCP2515::Init()
{

}

uint8_t CanMessageSenderMCP2515::SendMessage(uint16_t canId, uint8_t ext, uint8_t sizeOfByteArray, uint8_t *byteArray)
{
    can_frame tx_frame;

    tx_frame.can_id = canId;
    tx_frame.can_dlc = sizeOfByteArray;

    for (size_t i = 0; i < sizeOfByteArray; i++)
    {
        tx_frame.data[i] = byteArray[i];
    }

    //xSemaphoreTake(canSemaphore, portMAX_DELAY);
    can->sendMessage(&tx_frame);
    //xSemaphoreGive(canSemaphore);

    return 1;
}

void CanMessageSenderMCP2515::ReadMessage(uint16_t *canId, uint8_t *len, uint8_t *buf)
{
    *canId = 0;
    *len = 0;

    can_frame rx_frame;

    MCP2515::ERROR readResult = can->readMessage(&rx_frame);
    //Serial.println(readResult);
    if (readResult == MCP2515::ERROR_OK)
    {
        *canId = rx_frame.can_id;
        *len = rx_frame.can_dlc;
        for (int i = 0; i < rx_frame.can_dlc; i++)
        {
            buf[i] = rx_frame.data[i];
            //printf("0x%02X ", rx_frame.data.u8[i]);
        }
    }
}
