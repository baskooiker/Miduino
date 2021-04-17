#include "message_handlers.h"

#include "lcd_consts.h"

namespace Vleerhond
{
    void sendOscMessage(ofxOscSender& osc_sender, const std::string& address, const std::string& id, const uint8_t value)
    {
        // ofLogNotice("OSC", "Send message to %s: %s %d", address.c_str(), id.c_str(), value);
        ofxOscMessage m;
        m.setAddress(address);
        m.addStringArg(id);
        m.addIntArg(value);
        osc_sender.sendMessage( m );
    }

    void sendBass(ofxOscSender& osc_sender, const std::string& id, const uint8_t value)
    {
        sendOscMessage(osc_sender, "/bass", id, value);
    }

    void sendLead(ofxOscSender& osc_sender, const std::string& id, const uint8_t value)
    {
        ofLogNotice("OSC", "Send Lead, %s %d", id.c_str(), value);
        sendOscMessage(osc_sender, "/lead", id, value);
    }

    void handleNoteOnMessage(ofxOscSender& osc_sender, const uint8_t channel, const uint8_t pitch, const uint8_t velocity)
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

    void sendChordMode(ofxOscSender& osc_sender, const std::string& message)
    {
        // ofLogNotice("OSC", "Send ChordMode: %s", mesage.c_str());
        ofxOscMessage m;
        m.setAddress( "/chord_pattern" );
        m.addStringArg(message);
        osc_sender.sendMessage( m );
    }

    void handleChordModeMessage(ofxOscSender& osc_sender, const uint8_t channel, const uint8_t pitch)
    {
        switch (pitch)
        {
            case BUTTON_0:
                sendChordMode(osc_sender, std::string("STATIC_ROOT"));
                break;
            case BUTTON_1:
                sendChordMode(osc_sender, std::string("STATIC_NON_ROOT"));
                break;
            case BUTTON_2:
                sendChordMode(osc_sender, std::string("SIMPLE_PROGRESSION"));
                break;
            case BUTTON_3:
                sendChordMode(osc_sender, std::string("LONG_PROGRESSION"));
                break;
            default:
                // ofLogNotice("CHORD_MODE", "Unexpected chord_mode message: %d %d", channel, pitch);
                break;
        }
    }

    void handlePatternModeMessage(ofxOscSender& osc_sender, const uint8_t channel, const uint8_t pitch)
    {
        switch (pitch)
        {
            case BUTTON_0:
                sendBass(osc_sender, std::string("pattern"), 0);
                break;
            case BUTTON_1:
                sendBass(osc_sender, std::string("pattern"), 1);
                break;
            case BUTTON_2:
                sendBass(osc_sender, std::string("pattern"), 2);
                break;
            case BUTTON_3:
                sendBass(osc_sender, std::string("pattern"), 3);
                break;
            case BUTTON_4:
                sendLead(osc_sender, std::string("pattern"), 0);
                break;
            case BUTTON_5:
                sendLead(osc_sender, std::string("pattern"), 1);
                break;
            case BUTTON_6:
                sendLead(osc_sender, std::string("pattern"), 2);
                break;
            case BUTTON_7:
                sendLead(osc_sender, std::string("pattern"), 3);
                break;
            default:
                // ofLogNotice("CHORD_MODE", "Unexpected chord_mode message: %d %d", channel, pitch);
                break;
        }
    }

    void handleNoteOffMessage(ofxOscSender& osc_sender, ControlMode control_mode, const uint8_t channel, const uint8_t pitch)
    {
        switch(control_mode)
        {
            case ControlMode::CHORD_MODE:
                handleChordModeMessage(osc_sender, channel, pitch);
                break;
            case ControlMode::PATTERN_MODE:
                handlePatternModeMessage(osc_sender, channel, pitch);
                break;
            default:
                break;
        }
    }

}