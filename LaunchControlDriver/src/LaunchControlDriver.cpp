#include "LaunchControlDriver.h"

#include "ofLog.h"
#include "ofxMidiOut.h"
#include "ofxMidiIn.h"

#include "message_handlers.h"

namespace Vleerhond
{
    LaunchControlDriver::LaunchControlDriver()
        : midi_out("ofxMidiOutClient", MIDI_API_JACK)
        , midi_in("ofxMidiInClient", MIDI_API_JACK)
    {
    }

    void LaunchControlDriver::setup()
    {
        ofLogToConsole();

        osc_sender.setup("127.0.0.1", 4042);
        osc_receiver.setup(4043);

        std::string port_out_name = "Launch Control";
        uint8_t port_index = 0xFF;
        uint8_t i = 0;
        for (const std::string name : midi_out.getOutPortList())
        {
            ofLogNotice("MIDI", "Out port %d: %s", i, name.c_str());
            if(port_index == 0xFF)
            {
                if (name.find(port_out_name) != std::string::npos)
                {
                    port_index = i;
                    ofLogNotice("MIDI", "That's it!!!");
                }
            }
            i++;
        }
        if (port_index == 0xFF)
        {
            ofExit(-1);
        }
        midi_out.openPort(port_index);

        std::string port_in_name = "Launch Control";
        port_index = 0xFF;
        i = 0;
        for (const std::string name : midi_in.getInPortList())
        {
            ofLogNotice("MIDI", "In port %d: %s", i, name.c_str());
            if(port_index == 0xFF)
            {
                if (name.find(port_in_name) != std::string::npos)
                {
                    port_index = i;
                    ofLogNotice("MIDI", "That's it!!!");
                }
            }
            i++;
        }
        if (port_index == 0xFF)
        {
            ofExit(-1);
        }
        midi_in.openPort(port_index);
        midi_in.ignoreTypes(false, false, false);
        midi_in.setVerbose(true);
        midi_in.addListener(this);

        clock_counter = 0;

        // Reset button colors
        std::vector<uint8_t> bytes = {0xB0, 0x00, 0x00};
        midi_out.sendMidiBytes(bytes);
        }

    uint8_t getColorForPad(uint32_t counter, uint8_t pad_index, ControlMode control_mode)
    {
        uint32_t bar_number = counter / (24 * 4);
        bool active_step = bar_number % 8 == pad_index;
        switch(control_mode)
        {
            case ControlMode::CHORD_MODE:
                return active_step ? AMBER_LOW : AMBER_FULL;
            case ControlMode::PATTERN_MODE:
                return active_step ? GREEN_LOW : GREEN_FULL;
            default:
                return OFF_OFF;
        }
    }

    void LaunchControlDriver::receiveOscMessages()
    {
        // Receive clock counter
        while(osc_receiver.hasWaitingMessages())
        {
            // get the next message
            ofxOscMessage m;
            osc_receiver.getNextMessage(m);
            std::string message_address = m.getAddress();
            // ofLogNotice("OSC", "Received from %s", message_address.c_str());
            if ( message_address.compare("/clock") == 0 )
            {
                this->clock_counter = (uint64_t)m.getArgAsInt32(0);
                // ofLogNotice("OSC", "Received counter: %d", this->clock_counter);
            }
        }
    }

    void LaunchControlDriver::update()
    {
        receiveOscMessages();

        if (this->last_processed_counter != this->clock_counter || this->last_processed_mode != this->control_mode)
        {
            // Set pad LEDs
            for (uint8_t i = 0; i < 8; i++)
            {
                uint8_t note = BUTTONS[i];
                // ofLogNotice("LED", "note %d", note);
                uint8_t color_for_mode = getColorForPad((uint32_t)this->clock_counter, i, this->control_mode);
                // ofLogNotice("LED", "sendNoteOn(CHANNEL %d, note %d, color_for_mode %d)", CHANNEL, note, color_for_mode);
                midi_out.sendNoteOn(CHANNEL, note, color_for_mode);
            }
            this->last_processed_counter = clock_counter;
            this->last_processed_mode = this->control_mode;
        }

    }

    void LaunchControlDriver::draw() {}

    void LaunchControlDriver::keyPressed(int key) {}

    void LaunchControlDriver::handleCcMessage(const uint8_t channel, const uint8_t control, const uint8_t value)
    {
        switch (control)
        {
            case UP:
                if (value > 0)
                {
                    ofLogNotice("MIDIIN", "UP");
                }
                break;
            case DOWN:
                if (value > 0)
                {
                    ofLogNotice("MIDIIN", "DOWN");
                }
                break;
            case LEFT:
                if (value > 0)
                {
                    this->control_mode = ControlMode::CHORD_MODE;
                    ofLogNotice("MIDIIN", "Switch to Chord Mode");
                }
                break;
            case RIGHT:
                if (value > 0)
                {
                    this->control_mode = ControlMode::PATTERN_MODE;
                    ofLogNotice("MIDIIN", "Switch to Pattern Mode");
                }
                break;
            case ENCODER_B_1:
                sendBass(osc_sender, "density", value);
                break;
            case ENCODER_B_2:
                sendBass(osc_sender, "octave", value);
                break;
            case ENCODER_B_3:
                sendBass(osc_sender, "pitch_offset", value);
                break;
            case ENCODER_B_5:
                sendLead(osc_sender, "density", value);
                break;
            case ENCODER_B_6:
                // sendLead(osc_sender, "octave", value);
                break;
            case ENCODER_B_7:
                sendLead(osc_sender, "pitch_offset", value);
                break;
            default:
                ofLogNotice("MIDIIN", "CC(channel: %d, control: %d, value: %d)", channel, control, value);
                break;
        }
    }

    void LaunchControlDriver::newMidiMessage(ofxMidiMessage& message)
    {
        switch (message.status)
        {
        case MIDI_TIME_CLOCK:
            break;
        case MIDI_STOP:
            break;
        case MIDI_START:
        case MIDI_CONTINUE:
            break;
        case MIDI_NOTE_ON:
            handleNoteOnMessage(osc_sender, message.channel, message.pitch, message.velocity);
            break;
        case MIDI_NOTE_OFF:
            handleNoteOffMessage(osc_sender, this->control_mode, message.channel, message.pitch);
            break;
        case MIDI_CONTROL_CHANGE:
            handleCcMessage(message.channel, message.control, message.value);
            break;
        default:
            ofLogNotice("MIDIIN", "Unexpected status %d: channel %d, cc %d, value %d", message.status, message.channel, message.control, message.value);
            break;
        }
    }

    void LaunchControlDriver::keyReleased(int key) {}
    void LaunchControlDriver::mouseMoved(int x, int y) {}
    void LaunchControlDriver::mouseDragged(int x, int y, int button) {}
    void LaunchControlDriver::mousePressed(int x, int y, int button) {}
    void LaunchControlDriver::mouseReleased(int x, int y, int button) {}
    void LaunchControlDriver::mouseEntered(int x, int y) {}
    void LaunchControlDriver::mouseExited(int x, int y) {}
    void LaunchControlDriver::windowResized(int w, int h) {}
    void LaunchControlDriver::gotMessage(ofMessage msg) {}
    void LaunchControlDriver::dragEvent(ofDragInfo dragInfo) { }

    void LaunchControlDriver::exit()
    {
        ofLogNotice("", "Exit");
    }
}
