#pragma once

#include <string>

#include "ofxOsc.h"

#include "lcd_consts.h"

namespace Vleerhond
{
    void sendBass(ofxOscSender& osc_sender, const std::string& id, const uint8_t value);
    void sendLead(ofxOscSender& osc_sender, const std::string& id, const uint8_t value);

    void handleNoteOnMessage(ofxOscSender& osc_sender, const uint8_t channel, const uint8_t pitch, const uint8_t velocity);
    void handleNoteOffMessage(ofxOscSender& osc_sender, ControlMode control_mode, const uint8_t channel, const uint8_t pitch);
}
