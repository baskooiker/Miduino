#include "VleerhondApp.h"

#include "ofLog.h"
#include "ofxMidiOut.h"
#include "ofxMidiIn.h"

#include "callbacks.h"
#include "core/defs.h"
#include "midi_io.h"

namespace Vleerhond
{
    const char MODULE[] = "VleerhondApp";

    bool openFirstInput(const std::vector<std::string> midi_in_names, ofxMidiListener* listener)
    {
        bool in_found = false;
        for (std::string in_name : midi_in_names)
        {
            if (MidiIO::portAvailable(in_name))
            {
                if (MidiIO::setMainInput(in_name, listener))
                {
                    in_found = true;
                    break;
                }
                else
                {
                    return false;
                }

            }
        }
        if (!in_found)
        {
            return false;
        }
    }

    bool VleerhondApp::initializeMidiPorts()
    {
        std::string midi_a_name = "MIDISPORT 2x4 Out 1";
        std::string midi_b_name = "MIDISPORT 2x4 Out 2";
        std::string midi_c_name = "MIDISPORT 2x4 Out 3";
        std::string midi_d_name = "MIDISPORT 2x4 Out 4";

        if (!openFirstInput({ "Arturia BeatStep Pro", "MIDISPORT 2x4 In 2" }, this)) 
            return false;

        if (!MidiIO::addOutput(midi_a_name))
            return false;
        //if (!MidiIO::addOutput(midi_b_name)) 
        //    return false;
        //if (!MidiIO::addOutput(midi_c_name)) 
        //    return false;
        //if (!MidiIO::addOutput(midi_d_name)) 
        //    return false;

        return true;
    }

    void VleerhondApp::setup()
    {
        ofLogToConsole();

        if (!initializeMidiPorts())
        {
            ofExit(0);
        }

        // Init app
        data.randomize_all();

        MidiIO::sendProgramChange(MIDI_CHANNEL_DS, 8);

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

        if (!MidiIO::portsOpen())
        {
            ofExit(0);
        }
        else
        {
            data.processNoteEvents();

            if (data.time.state == PlayState::Stopped)
            {
                bool trigger_active = (int)ofGetCurrentTime().getAsSeconds() % 2 == 1;
                if (trigger_active != _is_trigger_on)
                {
                    MidiIO::send_cc(MINITAUR_CC_VCO2_WAVE, trigger_active ? 127 : 0, MIDI_CHANNEL_MINITAUR);
                    _is_trigger_on = trigger_active;
                    ofLogNotice("", "BLINK");
                }
            }
        }
    }

    void VleerhondApp::draw() {}

    void VleerhondApp::keyPressed(int key) {}

    void VleerhondApp::newMidiMessage(ofxMidiMessage& message)
    {
        switch (message.status)
        {
        case MIDI_TIME_CLOCK:
            MidiIO::sendTimeClock();
            handleClock(this->data);
            break;
        case MIDI_STOP:
            ofLogNotice("Vleerhond", "Stop!");
            handleStop(this->data);
            stop_counter++;
            if (stop_counter > 8)
            {
                ofLogNotice("stop_counter high");
                ofExit(shutdown_counter > 0 ? -1 : 0);
            }
            break;
        case MIDI_START:
        case MIDI_CONTINUE:
            stop_counter = 0;
            shutdown_counter = 0;
            ofLogNotice("Vleerhond", "Start!");
            data.time.state = PlayState::Playing;
            break;
        case MIDI_NOTE_ON:
            ofLogVerbose("MIDIIN", "NoteOn(d%, %d, %d)", message.channel, message.pitch, message.velocity);
            break;
        case MIDI_NOTE_OFF:
            break;
        case MIDI_CONTROL_CHANGE:
            ofLogVerbose("MIDIIN", "CC in: %d, %d", message.control, message.value);
            if (message.value == BTN_LEFT_BTM_08 && data.time.state == PlayState::Stopped)
            {
                shutdown_counter++;
            }
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
        ofLogNotice("", "Closing MIDI ports. shutdown_counter = %d", shutdown_counter);
        MidiIO::closeAll();
    }
}