#include "VleerhondApp.hpp"

#include "ofLog.h"
#include "ofxMidiOut.h"
#include "ofxMidiIn.h"

#include "callbacks.hpp"
#include "defs.hpp"
#include "midi_io.hpp"

namespace Vleerhond
{
    const char MODULE[] = "VleerhondApp";

    void MidiIO::send_note_on(const uint8_t pitch, const uint8_t velocity, const uint8_t channel)
    {
	for (ofxMidiOut& out: midi_out)
        {
	    out.sendNoteOn(channel, pitch, velocity);
        }
    }

    void MidiIO::send_note_off(const uint8_t pitch, const uint8_t channel)
    {
	for (ofxMidiOut& out: midi_out)
        {
            out.sendNoteOff(channel, pitch);
        }
    }

    void MidiIO::send_cc(uint8_t cc, uint8_t value, uint8_t channel)
    {
	for (ofxMidiOut& out: midi_out)
        {
	    out.sendControlChange(channel, cc, value);
        }
    }

    bool open_port(ofxMidiOut& port, std::string name)
    {
        uint8_t num_ports = port.getNumOutPorts();

        int port_index = -1;
        for (int i = 0; i < num_ports; i++)
        {
            std::string port_name = port.getOutPortName(i);
            int eq = port_name.find(name);
            ofLogNotice("midi out", "%s, num_ports = %d, eq = %d", port_name.c_str(), num_ports, eq);
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
            ofLogNotice("midi in", "%s, num_ports = %d, eq = %d", port_name.c_str(), num_ports, eq);
            if (eq >= 0)
            {
                ofLogNotice("midi in", port_name);
                return port.openPort(i);
            }
        }
        return false;
    }

    bool ports_open(std::vector<ofxMidiIn>& midi_in, std::vector<ofxMidiOut>& midi_out)
    {
	for (ofxMidiIn& in: midi_in)
        {
	    if(!in.isOpen())
            {
                return false;
            }
        }
	for (ofxMidiOut& out: midi_out)
        {
	    if(!out.isOpen())
            {
                return false;
            }
        }
        return true;
    }

    void initialize_midi_ports(std::vector<ofxMidiIn>& ins, std::vector<ofxMidiOut>& outs)
    {
        ofSetBackgroundColor(0);

        std::string midi_a_name = "MIDISPORT 2x2 Anniversary Out A";
        std::string midi_b_name = "MIDISPORT 2x2 Anniversary Out B";
        std::string midi_c_name = "MIDISPORT 2x2 Anniversary Out A";
        std::string midi_d_name = "MIDISPORT 2x2 Anniversary Out B";
        midi_a_name = "MidiSport 2x4:MidiSport 2x4 MIDI 1";
        midi_b_name = "MidiSport 2x4:MidiSport 2x4 MIDI 2";
        midi_c_name = "MidiSport 2x4:MidiSport 2x4 MIDI 3";
        midi_d_name = "MidiSport 2x4:MidiSport 2x4 MIDI 4";
        midi_a_name = "ESI M4U MIDI 1";
        midi_b_name = "ESI M4U MIDI 2";
        midi_c_name = "ESI M4U MIDI 3";
        midi_d_name = "ESI M4U MIDI 4";

        std::string midi_in_name = "ZeRO MkII";
        midi_in_name = "MidiSport 2x4:MidiSport 2x4 MIDI 1";
        midi_in_name = "ESI M4U MIDI 1";

        ins.push_back(ofxMidiIn());

	outs.push_back(ofxMidiOut());
	outs.push_back(ofxMidiOut());
	outs.push_back(ofxMidiOut());
	outs.push_back(ofxMidiOut());

        open_port(ins[0], midi_in_name);
        open_port(outs[0], midi_a_name);
        open_port(outs[1], midi_b_name);
        open_port(outs[2], midi_c_name);
        open_port(outs[3], midi_d_name);

        if (!ports_open(midi_in, midi_out))
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

        initialize_midi_ports(midi_in, midi_out);

        // don't ignore sysex, timing, & active sense messages,
        // these are ignored by default
        midi_in[0].ignoreTypes(false, false, false);
        midi_in[0].addListener(this);
        midi_in[0].setVerbose(true);

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
        bool MIDI_SETUP = true;
        if (!ports_open(midi_in, midi_out) && MIDI_SETUP)
        {
            ofSleepMillis(500);
            initialize_midi_ports(midi_in, midi_out);
            ofExit();
        }
        else
        {
            data.process_events();
        }
    }

    void VleerhondApp::draw() {}

    void VleerhondApp::keyPressed(int key) {
        switch (key)
        {
        case 'q':
            ofExit();
            break;
        case 'r':
            ofLogNotice(MODULE, "Randomize");
            data.randomize_all();
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
            for (ofxMidiOut& out: midi_out)
	    {
                out.sendMidiBytes(message.bytes);
            }
            //midi_out_a.sendMidiBytes(message.bytes);
            //midi_out_b.sendMidiBytes(message.bytes);
            //midi_out_c.sendMidiBytes(message.bytes);
            //midi_out_d.sendMidiBytes(message.bytes);
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
	for (ofxMidiOut& out: midi_out)
	{
            out.closePort();
        }
        for (ofxMidiIn& in: midi_in)
	{
            in.closePort();
        }
        //midi_in.closePort();
        //midi_out_a.closePort();
        //midi_out_b.closePort();
        //midi_out_c.closePort();
        //midi_out_d.closePort();
    }
}
