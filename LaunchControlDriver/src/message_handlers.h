#pragma once

#include <string>

#include "ofxOsc.h"

#include "lcd_consts.h"

namespace Vleerhond
{
    void sendBass(std::queue<ofxOscMessage>& message_queue, const std::string& id, const uint8_t value);
    void sendLead(std::queue<ofxOscMessage>& message_queue, const std::string& id, const uint8_t value);

    void handleNoteOnMessage(std::queue<ofxOscMessage>& message_queue, const uint8_t channel, const uint8_t pitch, const uint8_t velocity);
    void handleNoteOffMessage(std::queue<ofxOscMessage>& message_queue, ControlMode control_mode, const uint8_t channel, const uint8_t pitch);
}
