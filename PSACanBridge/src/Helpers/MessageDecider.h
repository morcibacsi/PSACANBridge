#pragma once

#ifndef _MessageDecider_h
    #define _MessageDecider_h

#include "../SerialPort/AbstractSerial.h"
#include "../../Config.h"
#include "../Helpers/DataBroker.h"
#include <list>

constexpr uint8_t SHOULD_NOT_FORWARD_TO_2010 = 0;
constexpr uint8_t SHOULD_FORWARD_TO_2010 = 1;
constexpr uint8_t SHOULD_MODIFY_FOR_2010 = 2;

class MessageDecider {

std::list<uint16_t> _msgToProcess;
std::list<uint16_t> _msgToSkip;
std::list<uint16_t> _msgToForward;

public:
    MessageDecider();

    uint8_t ShouldProcess(uint16_t canId);
};

#endif
