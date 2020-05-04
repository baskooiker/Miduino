#include "VleerhondApp.h"

#include "ofLog.h"
#include "ofxMidiOut.h"
#include "ofxMidiIn.h"

#include "callbacks.h"
#include "core/defs.h"
#include "midi_io.h"
#include "midi/console_midi_channel.h"

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
        if (!openFirstInput({ "Arturia BeatStep Pro", "MIDISPORT 2x4 In 2" }, this)) 
            return false;

        if (!MidiIO::addOutput(MIDI_A_NAME, 0))
            return false;
        if (!MidiIO::addOutput(MIDI_B_NAME, 0)) 
            return false;
        if (!MidiIO::addOutput(MIDI_C_NAME, 4)) 
            return false;
        if (!MidiIO::addOutput(MIDI_D_NAME, 0)) 
            return false;

        return true;
    }

    void VleerhondApp::setup()
    {
        ofLogToConsole();

        if (false)
        {
            for (int i = 0; i < 100; i++)
            {
                data.randomizeAll();
            }
            ofExit(0);
            return;

            data.harmony.randomize();
            data.harmony.setType(HarmonyType::Const);
            std::shared_ptr<ConsoleMidiChannel> console_midi_channel = std::make_shared<ConsoleMidiChannel>(MIDI_A_NAME);

            data.vermona.fugue.randomize();
            data.vermona.select(1);
            data.vermona.setChannel(console_midi_channel);

            int bars = 1;
            for (int i = 0; i < bars * 16 * TICKS_PER_STEP; i++)
            {
                // From clock callback
                if (Utils::intervalHit(TimeDivision::Sixteenth, data.time))
                {
                    data.updatePedalState();
                }
                data.processActiveNotes();
                data.playAll();
                data.time.tick += 1;
            }


            // Flush active notes
            for (int i = 0; i < (16 * 6); i++)
            {
                data.processActiveNotes();
            }

            console_midi_channel->printStorage();
            console_midi_channel->print();

            ofExit(0);
            return;
        }

        if (!initializeMidiPorts())
        {
            ofExit(0);
        }

        data.connect();

        // Init app
        data.randomizeAll();

        // Set initial 808 program
        data.drumstation.reset();

        data.vermona.select(1);
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

            // If stopped, blink light on moog
            if (data.time.state == PlayState::Stopped)
            {
                bool trigger_active = (int)ofGetCurrentTime().getAsSeconds() % 2 == 1;
                if (trigger_active != _is_trigger_on)
                {
                    data.minitaur.bass_root.setVco2Square(trigger_active);
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
                ofExit(0);
            }
            break;
        case MIDI_START:
        case MIDI_CONTINUE:
            stop_counter = 0;
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
        ofLogNotice("", "Closing MIDI ports");
        MidiIO::closeAll();
    }
}