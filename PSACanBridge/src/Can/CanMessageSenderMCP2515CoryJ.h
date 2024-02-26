#pragma once

#ifndef _CanMessageSenderMCP2515CoryJ_h
    #define _CanMessageSenderMCP2515CoryJ_h

#include <mcp_can.h>
#include "ICanMessageSender.h"
#include "../SerialPort/AbstractSerial.h"

class CanMessageSenderMCP2515CoryJ : public ICanMessageSender
{
private:
    MCP_CAN *can;
    uint8_t _csPin;
    uint8_t _canSpeed;
    uint8_t _canClock;
    SPIClass *_spi;
    AbsSer *_serialPort;

public:
    CanMessageSenderMCP2515CoryJ(uint8_t csPin, uint8_t canSpeed, uint8_t canClock, SPIClass *spi, AbsSer *serialPort);
    void Init() override;
    uint8_t SendMessage(uint16_t canId, uint8_t ext, uint8_t sizeOfByteArray, uint8_t *byteArray) override;
    virtual bool ReadMessage(uint16_t *canId, uint8_t *len, uint8_t *buf) override;
    virtual void Reset() override;
};

#endif
