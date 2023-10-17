#include "CanMessageSenderMCP2515CoryJ.h"
#include "Arduino.h"

CanMessageSenderMCP2515CoryJ::CanMessageSenderMCP2515CoryJ(uint8_t csPin, uint8_t canSpeed, uint8_t canClock, SPIClass *spi, AbsSer *serialPort)
{
    _csPin = csPin;
    _canSpeed = canSpeed;
    _canClock = canClock;
    _spi = spi;
    _serialPort = serialPort;

    Reset();
}

void CanMessageSenderMCP2515CoryJ::Init()
{

}

uint8_t CanMessageSenderMCP2515CoryJ::SendMessage(uint16_t canId, uint8_t ext, uint8_t sizeOfByteArray, uint8_t *byteArray)
{
    can->sendMsgBuf(canId, ext, sizeOfByteArray, byteArray);

    return 1;
}

bool CanMessageSenderMCP2515CoryJ::ReadMessage(uint16_t *canId, uint8_t *len, uint8_t *buf)
{
    *canId = 0;
    *len = 0;

    unsigned long rxId;
    byte length;
    byte rxBuf[8];

    //if (can->checkReceive() == CAN_MSGAVAIL){
        byte readResult = can->readMsgBuf(&rxId, &length, buf);

        if (readResult == CAN_OK)
        {
            *canId = rxId;
            *len = length;
            return true;
        }
        else
        {
            uint8_t err = can->checkError();
            //_serialPort->print("CAN2004 receive error: 0x");
            //_serialPort->println(err, HEX);

            uint8_t err2 = can->getError();
            //_serialPort->print("CAN2004 receive error2: 0x");
            //_serialPort->println(err2, HEX);

            return err == CAN_OK;
        }
    //}

    return false;
}

void CanMessageSenderMCP2515CoryJ::Reset()
{
    can = new MCP_CAN(_spi, _csPin);
    can->begin(MCP_ANY, _canSpeed, _canClock);

    can->setMode(MCP_NORMAL);
}