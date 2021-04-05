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
            ofLogNotice("", "Trying to open port %s", in_name.c_str());
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

        return true;
    }

    bool VleerhondApp::initializeMidiPorts()
    {
        if (!openFirstInput({"ttymidi"}, this))
            return false;

        if (!MidiIO::addOutput("ttymidi", 0))
        {
            ofLogNotice("", "Failed to open output!");
            return false;
        }
        ofLogNotice("", "Succesfully opened output!");

        return true;
    }

    void VleerhondApp::setup()
    {
        ofLogToConsole();

        receiver.setup(4041);

        if (!initializeMidiPorts())
        {
            ofExit(0);
        }

        data.connect();

        // Init app
        data.randomizeAll();

        data.vermona.select(0);
    }

    void VleerhondApp::update()
    {
        static bool _is_trigger_on = false;

        if (!MidiIO::portsOpen())
        {
            ofExit(0);
        }

        data.processNoteEvents();

	// check for waiting messages
	while( receiver.hasWaitingMessages() )
	{
            // get the next message
            ofxOscMessage m;
            receiver.getNextMessage( &m );
            ofLogNotice("OSC", "Received from %s", m.getAddress().c_str());
            if ( strcmp( m.getAddress().c_str(), "/mouse/position" ) == 0 )
            {
            }
        }

        // If stopped, blink light on moog
        if (data.time.state == PlayState::Stopped)
        {
            bool trigger_active = (int)ofGetCurrentTime().getAsSeconds() % 2 == 1;
            if (trigger_active != _is_trigger_on)
            {
                data.minitaur.bass_root.setVco2Square(trigger_active);
                _is_trigger_on = trigger_active;
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
            else if (stop_counter > 1)
            {
                for (InstrumentBase* inst : data.getInstrumentPtrs())
                {
                    inst->getChannel()->allNotesOff();
                }
            }
            break;
        case MIDI_START:
        case MIDI_CONTINUE:
            stop_counter = 0;
            ofLogNotice("Vleerhond", "Start!");

            data.randomizeAll();

            data.neutron.select(2);
            data.neutron.setVariablePitchOffset(32);

            data.minitaur.select(1);
            data.minitaur.setVariablePitchOffset(64);

            data.time.state = PlayState::Playing;
            break;
        case MIDI_NOTE_ON:
            ofLogNotice("MIDIIN", "NoteOn(d%, %d, %d)", message.channel, message.pitch, message.velocity);
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
