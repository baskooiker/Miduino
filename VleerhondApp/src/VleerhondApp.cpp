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

    bool VleerhondApp::stateOk()
    {
        return state;
    }

    bool VleerhondApp::initializeMidiPorts()
    {
        std::cout << "Starting setup\n";
        std::string midi_a_name = "f_midi";
        std::string midi_b_name = "f_midi";
        std::string midi_c_name = "f_midi";
        std::string midi_d_name = "f_midi";

        std::string midi_in_name = "f_midi";

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

        return success;
    }

    void VleerhondApp::setup()
    {
        state = true;
        ofLogToConsole();

        if (!initializeMidiPorts())
        {
            state = false;
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
            state = false;
        }
    }

    void VleerhondApp::draw() {}

    void VleerhondApp::keyPressed(int key) {
        switch (key)
        {
        case 'q':
            ofExit();
            state = false;
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
        std::cout << "newMidiMessage\n";
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
