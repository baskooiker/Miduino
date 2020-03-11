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

            data.harmony.randomize();
            data.harmony.setType(HarmonyType::Const);
            std::shared_ptr<ConsoleMidiChannel> console_midi_channel = std::make_shared<ConsoleMidiChannel>(MIDI_A_NAME);

            if (false)
            {
                data.mam_mb33.setChannel(console_midi_channel);
                data.mam_mb33.randomize();
                data.mam_mb33.setVariableDensity(127);
                data.mam_mb33.setVariablePitchOffset(64);
                data.mam_mb33.select(2);
            }
            if (true)
            {
                data.neutron.setChannel(console_midi_channel);
                data.neutron.select(3);
                data.neutron.setVariablePitchOffset(127);
            }

            int bars = 1;
            for (int i = 0; i < bars * 16 * TICKS_PER_STEP; i++)
            {
                // From clock callback
                if (Utils::interval_hit(TimeDivision::Sixteenth, data.time))
                {
                    data.updatePedalState();
                }
                data.process_active_notes();
                data.play_all();
                data.time.tick += 1;
            }


            // Flush active notes
            for (int i = 0; i < (16 * 6); i++)
            {
                data.process_active_notes();
            }

            console_midi_channel->print_storage();
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
        data.randomize_all();

        // Set initial 808 program
        data.drumstation.reset();
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
                    data.moog_bass.setVco2Square(trigger_active);
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
            if (message.value == BSP_PAD_TOP_08 && data.time.state == PlayState::Stopped)
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