#include "VleerhondApp.h"

#define NOVATION_ZERO_SL

#include "ofLog.h"
#include "ofxMidiOut.h"
#include "ofxMidiIn.h"

#include "callbacks.h"
#include "defs.h"
#include "midi_io.h"

ofxMidiOut midi_out_a;
ofxMidiOut midi_out_b;
ofxMidiOut midi_out_c;
ofxMidiOut midi_out_d;
ofxMidiIn midi_in;

namespace Vleerhond
{
    const char MODULE[] = "VleerhondApp";

    void MidiIO::send_note_on(const uint8_t pitch, const uint8_t velocity, const uint8_t channel)
    {
        midi_out_a.sendNoteOn(channel, pitch, velocity);
        midi_out_b.sendNoteOn(channel, pitch, velocity);
        midi_out_c.sendNoteOn(channel, pitch, velocity);
        midi_out_d.sendNoteOn(channel, pitch, velocity);
        switch (channel)
        {
        case MIDI_CHANNEL_TANZBAR:
            switch (pitch)
            {
            case NOTE_TANZBAR_HT:
                //printf("Hi tom\n");
                break;
            case NOTE_TANZBAR_MT:
                //printf("Mid tom\n");
                break;
            case NOTE_TANZBAR_LT:
                //printf("Lo tom\n");
                break;
            case NOTE_TANZBAR_HC:
                //printf("Hi Conga\n");
                break;
            case NOTE_TANZBAR_MC:
                //printf("Mid Conga\n");
                break;
            case NOTE_TANZBAR_LC:
                //printf("Lo Conga\n");
                break;
            }
            break;
        }
    }

    void MidiIO::send_note_off(const uint8_t pitch, const uint8_t channel)
    {
        midi_out_a.sendNoteOff(channel, pitch);
        midi_out_b.sendNoteOff(channel, pitch);
        midi_out_c.sendNoteOff(channel, pitch);
        midi_out_d.sendNoteOff(channel, pitch);
    }

    void MidiIO::send_cc(uint8_t cc, uint8_t value, uint8_t channel)
    {
        midi_out_a.sendControlChange(channel, cc, value);
        midi_out_b.sendControlChange(channel, cc, value);
        midi_out_c.sendControlChange(channel, cc, value);
        midi_out_d.sendControlChange(channel, cc, value);
    }

#define NOVATION_ZERO_SL

    bool open_port(ofxMidiOut& port, std::string name)
    {
        uint8_t num_ports = port.getNumOutPorts();

        int port_index = -1;
        for (int i = 0; i < num_ports; i++)
        {
            std::string port_name = port.getOutPortName(i);
            int eq = port_name.find(name);
            ofLogNotice("midi out", "%s, num_ports = %d", port_name.c_str(), num_ports);
            if (eq >= 0)
            {
                ofLogNotice("midi out", port_name);
                return port.openPort(i);
            }
        }
        return false;
    }

    bool open_port(ofxMidiIn& port, std::string name)
    {
        uint8_t num_ports = port.getNumInPorts();

        int port_index = -1;
        for (int i = 0; i < num_ports; i++)
        {
            std::string port_name = port.getInPortName(i);
            int eq = port_name.find(name);
            ofLogNotice("midi in", "%s, num_ports = %d", port_name.c_str(), num_ports);
            if (eq >= 0)
            {
                ofLogNotice("midi in", port_name);
                return port.openPort(i);
            }
        }
        return false;
    }

    bool ports_open()
    {
        return midi_in.isOpen()
            && midi_out_a.isOpen()
            && midi_out_b.isOpen()
            && midi_out_c.isOpen()
            && midi_out_d.isOpen();
    }

    void initialize_midi_ports()
    {
        ofSetBackgroundColor(0);

        std::string midi_a_name = "MIDISPORT 2x2 Anniversary Out A";
        std::string midi_b_name = "MIDISPORT 2x2 Anniversary Out B";
        std::string midi_c_name = "MIDISPORT 2x2 Anniversary Out A";
        std::string midi_d_name = "MIDISPORT 2x2 Anniversary Out B";
        midi_a_name = "MIDISPORT 2x4 Out 1";
        midi_b_name = "MIDISPORT 2x4 Out 2";
        midi_c_name = "MIDISPORT 2x4 Out 3";
        midi_d_name = "MIDISPORT 2x4 Out 4";

        std::string midi_in_name = "ZeRO MkII";
        midi_in_name = "MIDISPORT 2x4 In 1";

        open_port(midi_in, midi_in_name);
        open_port(midi_out_a, midi_a_name);
        open_port(midi_out_b, midi_b_name);
        open_port(midi_out_c, midi_c_name);
        open_port(midi_out_d, midi_d_name);

        if (!ports_open())
        {
            ofSetBackgroundColor(0);
        }
        else
        {
            ofSetBackgroundColor(128);
        }

    }

    void VleerhondApp::setup()
    {
        ofLogToConsole();

        initialize_midi_ports();

        // don't ignore sysex, timing, & active sense messages,
        // these are ignored by default
        midi_in.ignoreTypes(false, false, false);
        midi_in.addListener(this);
        midi_in.setVerbose(true);

        // Init app
        data.randomize_all();
    }

    void VleerhondApp::play()
    {
        data.time.state = PlayState::Playing;
        data.process_active_notes();
        data.play_all();
        data.time.tick++;
    }

    void VleerhondApp::update()
    {
        static bool _is_trigger_on = false;

        if (!ports_open())
        {
            ofSleepMillis(500);
            initialize_midi_ports();
        }
        else
        {
            data.process_events();

            if (data.time.state == PlayState::Stopped)
            {
                bool trigger_active = (int)ofGetCurrentTime().getAsSeconds() % 2 == 1;
                if (trigger_active != _is_trigger_on)
                {
                    MidiIO::send_cc(MINITAUR_CC_VCO2_WAVE, trigger_active ? 127 : 0, MIDI_CHANNEL_MINITAUR);
                    _is_trigger_on = trigger_active;
                }
            }
        }
    }

    void VleerhondApp::draw() {}

    void VleerhondApp::keyPressed(int key) {
        switch (key)
        {
        case 'q':
            ofExit();
            break;
        case 'p':
            break;
        case 's':
            ofLogNotice(MODULE, "Stop.");
            data.time.state = PlayState::Stopped;
            break;
        case 'r':
            ofLogNotice(MODULE, "Randomize");
            data.randomize_all();
            break;
        case 'i':
            initialize_midi_ports();
            break;
        case 'z':
            break;
        case 'x':
            break;
        case 'c':
            break;
        default:
            break;
        }
    }

    void VleerhondApp::newMidiMessage(ofxMidiMessage& message)
    {
        //ofLogNotice("App", message.getStatusString(message.status).c_str());
        switch (message.status)
        {
        case MIDI_TIME_CLOCK:
            midi_out_a.sendMidiBytes(message.bytes);
            midi_out_b.sendMidiBytes(message.bytes);
            midi_out_c.sendMidiBytes(message.bytes);
            midi_out_d.sendMidiBytes(message.bytes);
            handleClock(this->data);
            break;
        case MIDI_STOP:
            ofLogNotice("Vleerhond", "Stop!");
            handleStop(this->data);
            break;
        case MIDI_START:
            ofLogNotice("Vleerhond", "Start!");
            data.time.state = PlayState::Playing;
            break;
        case MIDI_NOTE_ON:
            break;
        case MIDI_NOTE_OFF:
            break;
        case MIDI_CONTROL_CHANGE:
            ofLogNotice("MIDIIN", "CC in: %d, %d", message.control, message.value);
            handleControlChange(this->data, message.channel, message.control, message.value);
            break;
        default:
            break;
        }
    }

    void VleerhondApp::keyReleased(int key) {}
    void VleerhondApp::mouseMoved(int x, int y) {}
    void VleerhondApp::mouseDragged(int x, int y, int button) {}
    void VleerhondApp::mousePressed(int x, int y, int button) {}
    void VleerhondApp::mouseReleased(int x, int y, int button) {}
    void VleerhondApp::mouseEntered(int x, int y) {}
    void VleerhondApp::mouseExited(int x, int y) {}
    void VleerhondApp::windowResized(int w, int h) {}
    void VleerhondApp::gotMessage(ofMessage msg) {}
    void VleerhondApp::dragEvent(ofDragInfo dragInfo) { }

    void VleerhondApp::exit()
    {
        midi_in.closePort();
        midi_out_a.closePort();
        midi_out_b.closePort();
        midi_out_c.closePort();
        midi_out_d.closePort();
    }
}