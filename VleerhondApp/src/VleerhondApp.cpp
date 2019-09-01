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

    bool VleerhondApp::initializeMidiPorts()
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

        bool success = true;

        success &= MidiIO::addMidiIn(midi_in_name);
        // don't ignore sysex, timing, & active sense messages,
        // these are ignored by default
        MidiIO::ins()[0].ignoreTypes(false, false, false);
        MidiIO::ins()[0].addListener(this);
        MidiIO::ins()[0].setVerbose(true);


        success &= MidiIO::addMidiOut(midi_a_name);
        success &= MidiIO::addMidiOut(midi_b_name);
        success &= MidiIO::addMidiOut(midi_c_name);
        success &= MidiIO::addMidiOut(midi_d_name);

        if (success)
        {
            ofSetBackgroundColor(128);
        }
        else
        {
            ofSetBackgroundColor(0);
        }
        return success;
    }

    void VleerhondApp::setup()
    {
        ofLogToConsole();

        if (!initializeMidiPorts())
        {
            ofExit();
        }

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
        if (MidiIO::portsOpen())
        {
            data.process_events();
        }
        else
        {
            ofExit();
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
        switch (message.status)
        {
        case MIDI_TIME_CLOCK:
            for (ofxMidiOut& out : MidiIO::outs())
            {
                out.sendMidiBytes(message.bytes);
            }
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

    void VleerhondApp::exit()
    {
        for (ofxMidiOut& out : MidiIO::outs())
        {
            out.closePort();
        }
        for (ofxMidiIn& in : MidiIO::ins())
        {
            in.closePort();
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
}
