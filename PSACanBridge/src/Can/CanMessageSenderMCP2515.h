#pragma once

#ifndef _CanMessageSenderMCP2515_h
    #define _CanMessageSenderMCP2515_h

#include <mcp2515.h>
#include "ICanMessageSender.h"

class CanMessageSenderMCP2515 : public ICanMessageSender
{
private:
    MCP2515 *can;
    uint8_t _csPin;
    CAN_SPEED _canSpeed;
    CAN_CLOCK _canClock;
    SPIClass *_spi;

public:
    CanMessageSenderMCP2515(uint8_t csPin, CAN_SPEED canSpeed, CAN_CLOCK canClock, SPIClass *spi);
    void Init() override;
    uint8_t SendMessage(uint16_t canId, uint8_t ext, uint8_t sizeOfByteArray, uint8_t *byteArray) override;
    virtual bool ReadMessage(uint16_t *canId, uint8_t *len, uint8_t *buf) override;
    virtual void Reset() override;
};

#endif
