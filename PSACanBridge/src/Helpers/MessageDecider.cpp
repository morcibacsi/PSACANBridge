#include "MessageDecider.h"

MessageDecider::MessageDecider()
{
    // MSG to process
    _msgToProcess.push_back(0x0B6);
    _msgToProcess.push_back(0x0E6);
    _msgToProcess.push_back(0x0F6);
    _msgToProcess.push_back(0x1A1);
    _msgToProcess.push_back(0x036);
    _msgToProcess.push_back(0x3A7); // -> 0x3E7
    _msgToProcess.push_back(0x127);
    _msgToProcess.push_back(0x128);
    _msgToProcess.push_back(0x161);
    _msgToProcess.push_back(0x168);
    _msgToProcess.push_back(0x1A8); // -> 0x228
    _msgToProcess.push_back(0x220);
    //_msgToProcess.push_back(0x221);
    _msgToProcess.push_back(0x217);
    _msgToProcess.push_back(0x227);
    _msgToProcess.push_back(0x361);
    _msgToProcess.push_back(0x21F);

    // MSG to skip
    _msgToSkip.push_back(0x260);
    //_msgToSkip.push_back(0x220);
    //VIN - we send the VIN which is set in the config
    _msgToSkip.push_back(0x2B6);
    _msgToSkip.push_back(0x3B6);
    _msgToSkip.push_back(0x336);
}

uint8_t MessageDecider::ShouldProcess(uint16_t canId)
{
    bool foundInToBeProcessed = (std::find(_msgToProcess.begin(), _msgToProcess.end(), canId) != _msgToProcess.end());
    if (foundInToBeProcessed)
    {
        return SHOULD_MODIFY_FOR_2010;
    }

    bool foundInToBeSkipped = (std::find(_msgToSkip.begin(), _msgToSkip.end(), canId) != _msgToSkip.end());
    if (foundInToBeSkipped)
    {
        return SHOULD_NOT_FORWARD_TO_2010;
    }

    return SHOULD_FORWARD_TO_2010;
}
