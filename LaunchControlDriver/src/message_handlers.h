#pragma once

#include <map>
#include <string>

#include "ofxOsc.h"

#include "lcd_consts.h"

namespace Vleerhond
{
    void sendBass(std::map<std::string, ofxOscMessage>& message_queue, const std::string& id, const uint8_t value);
    void sendLead(std::map<std::string, ofxOscMessage>& message_queue, const std::string& id, const uint8_t value);

    void handleNoteOnMessage(std::map<std::string, ofxOscMessage>& message_queue, const uint8_t channel, const uint8_t pitch, const uint8_t velocity);
    void handleNoteOffMessage(std::map<std::string, ofxOscMessage>& message_queue, ControlMode control_mode, const uint8_t channel, const uint8_t pitch);
}
