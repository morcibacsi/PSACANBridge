#ifdef AutowpCAN

#include "CanMessageSenderMCP2515.h"
#include "Arduino.h"

CanMessageSenderMCP2515::CanMessageSenderMCP2515(uint8_t csPin, CAN_SPEED canSpeed, CAN_CLOCK canClock, SPIClass *spi)
{
    _csPin = csPin;
    _canSpeed = canSpeed;
    _canClock = canClock;
    _spi = spi;

    can = new MCP2515(_csPin, spi);
    Reset();
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

    can->sendMessage(&tx_frame);

    return 1;
}

bool CanMessageSenderMCP2515::ReadMessage(uint16_t *canId, uint8_t *len, uint8_t *buf)
{
    *canId = 0;
    *len = 0;

    can_frame rx_frame;

    MCP2515::ERROR readResult = can->readMessage(&rx_frame);

    if (readResult == MCP2515::ERROR_OK)
    {
        *canId = rx_frame.can_id;
        *len = rx_frame.can_dlc;
        for (int i = 0; i < rx_frame.can_dlc; i++)
        {
            buf[i] = rx_frame.data[i];
        }

        return true;
    }else{
        uint8_t err = can->getErrorFlags();
        printf("CAN2004 receive error: 0x%02X \r\n", err);
    }
    return false;
}

void CanMessageSenderMCP2515::Reset()
{
    digitalWrite(_csPin, HIGH);
    _spi->endTransaction();

    can->reset();
    can->setBitrate(_canSpeed, _canClock);
    can->setNormalMode();
}
#endif