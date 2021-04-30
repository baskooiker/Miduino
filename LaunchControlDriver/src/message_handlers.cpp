#include "message_handlers.h"

#include "lcd_consts.h"

namespace Vleerhond
{
    void sendOscMessage(std::queue<ofxOscMessage>& message_queue, const std::string& address, const std::string& id, const uint8_t value)
    {
        // ofLogNotice("OSC", "Send message to %s: %s %d", address.c_str(), id.c_str(), value);
        ofxOscMessage m;
        m.setAddress(address);
        m.addStringArg(id);
        m.addIntArg(value);
        message_queue.push(m);
    }

    void sendBass(std::queue<ofxOscMessage>& message_queue, const std::string& id, const uint8_t value)
    {
        sendOscMessage(message_queue, "/bass", id, value);
    }

    void sendLead(std::queue<ofxOscMessage>& message_queue, const std::string& id, const uint8_t value)
    {
        ofLogNotice("OSC", "Send Lead, %s %d", id.c_str(), value);
        sendOscMessage(message_queue, "/lead", id, value);
    }

    void handleNoteOnMessage(std::queue<ofxOscMessage>& message_queue, const uint8_t channel, const uint8_t pitch, const uint8_t velocity)
    {
        switch (pitch)
        {
            case BUTTON_0:
                ofLogNotice("MIDIIN", "BUTTON[0] on");
                break;
            case BUTTON_1:
                ofLogNotice("MIDIIN", "BUTTON[1] on");
                break;
            case BUTTON_2:
                ofLogNotice("MIDIIN", "BUTTON[2] on");
                break;
            case BUTTON_3:
                ofLogNotice("MIDIIN", "BUTTON[3] on");
                break;
            case BUTTON_4:
                ofLogNotice("MIDIIN", "BUTTON[4] on");
                break;
            case BUTTON_5:
                ofLogNotice("MIDIIN", "BUTTON[5] on");
                break;
            case BUTTON_6:
                ofLogNotice("MIDIIN", "BUTTON[6] on");
                break;
            case BUTTON_7:
                ofLogNotice("MIDIIN", "BUTTON[7] on");
                break;
            default:
                ofLogNotice("MIDIIN", "NoteOn(channel: %d, pitch: %d, velocity: %d)", channel, pitch, velocity);
                break;
        }
    }

    void sendChordMode(std::queue<ofxOscMessage>& message_queue, const std::string& message)
    {
        // ofLogNotice("OSC", "Send ChordMode: %s", mesage.c_str());
        ofxOscMessage m;
        m.setAddress("/chord_pattern");
        m.addStringArg(message);
        message_queue.push(m);
    }

    void handleChordModeMessage(std::queue<ofxOscMessage>& message_queue, const uint8_t channel, const uint8_t pitch)
    {
        switch (pitch)
        {
            case BUTTON_0:
                sendChordMode(message_queue, std::string("STATIC_ROOT"));
                break;
            case BUTTON_1:
                sendChordMode(message_queue, std::string("STATIC_NON_ROOT"));
                break;
            case BUTTON_2:
                sendChordMode(message_queue, std::string("SIMPLE_PROGRESSION"));
                break;
            case BUTTON_3:
                sendChordMode(message_queue, std::string("LONG_PROGRESSION"));
                break;
            default:
                // ofLogNotice("CHORD_MODE", "Unexpected chord_mode message: %d %d", channel, pitch);
                break;
        }
    }

    void handlePatternModeMessage(std::queue<ofxOscMessage>& message_queue, const uint8_t channel, const uint8_t pitch)
    {
        switch (pitch)
        {
            case BUTTON_0:
                sendBass(message_queue, std::string("pattern"), 0);
                break;
            case BUTTON_1:
                sendBass(message_queue, std::string("pattern"), 1);
                break;
            case BUTTON_2:
                sendBass(message_queue, std::string("pattern"), 2);
                break;
            case BUTTON_3:
                sendBass(message_queue, std::string("pattern"), 3);
                break;
            case BUTTON_4:
                sendLead(message_queue, std::string("pattern"), 0);
                break;
            case BUTTON_5:
                sendLead(message_queue, std::string("pattern"), 1);
                break;
            case BUTTON_6:
                sendLead(message_queue, std::string("pattern"), 2);
                break;
            case BUTTON_7:
                sendLead(message_queue, std::string("pattern"), 3);
                break;
            default:
                // ofLogNotice("CHORD_MODE", "Unexpected chord_mode message: %d %d", channel, pitch);
                break;
        }
    }

    void handleNoteOffMessage(std::queue<ofxOscMessage>& message_queue, ControlMode control_mode, const uint8_t channel, const uint8_t pitch)
    {
        switch(control_mode)
        {
            case ControlMode::CHORD_MODE:
                handleChordModeMessage(message_queue, channel, pitch);
                break;
            case ControlMode::PATTERN_MODE:
                handlePatternModeMessage(message_queue, channel, pitch);
                break;
            default:
                break;
        }
    }

}