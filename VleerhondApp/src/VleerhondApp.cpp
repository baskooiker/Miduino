#include "VleerhondApp.h"

#include <stdlib.h>

#include "ofLog.h"
#include "ofxMidiOut.h"
#include "ofxMidiIn.h"

#include "callbacks.h"
#include "core/defs.h"
#include "midi_io.h"
#include "midi/console_midi_channel.h"
#include "application/nanokontrol2.h"
#include "osc_callbacks.h"

namespace Vleerhond
{
    const char MODULE[] = "VleerhondApp";

    bool openOutputPort(const std::string& target_port_name, ofxMidiOut& out)
    {
        for (int i = 0; i < out.getNumOutPorts(); i++)
        {
            if (out.getOutPortName(i).find(target_port_name) != -1)
            {
                if (out.openPort(i))
                {
                    return true;
                }
            }
        }
        return false;
    }

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
        //if (!openFirstInput({"ttymidi"}, this))
        if (!openFirstInput({"Midi Through Port-1"}, this))
        {
            return false;
        }

        if (!MidiIO::addOutput("ttymidi", 1))
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

        osc_receiver.setup(4042);
        osc_sender.setup("127.0.0.1", 4043);

        if (!openFirstInput({"nanoKONTROL2"}, this))
        {
            ofLogError("", "Failed to open input 'nanoKONTROL2'!");
            ::exit(-1);
        }

        if (!initializeMidiPorts())
        {
            ::exit(-1);
        }

        if(openOutputPort("Midi Through 14:1", midicloro_out))
        {
            ofLogNotice("", "Failed to open mdicloro output: 'Midi Through 14:1'!");
            ::exit(-1);
        }

        auto nanokontrol = std::make_shared<NanoKontrol2>(data, midicloro_out);
        midi_listeners.push_back(nanokontrol);

        data.connect();

        // Init app
        data.randomizeAll();

        // TODO: Send All note offs to all instruments

        data.vermona.select(0);

        // Set default key to G
        data.harmony.scale.setRoot(Root::ROOT_G);

        // Set default MS20 to ARP
        data.neutron.select(3);
    }

    void VleerhondApp::update()
    {
        static bool _is_trigger_on = false;

        if (!MidiIO::portsOpen())
        {
            ::exit(-1);
        }

        data.processNoteEvents();

        receiveOscMessages(data, osc_receiver);
    }

    void VleerhondApp::draw() {}

    void VleerhondApp::keyPressed(int key) {
        ofLogNotice("Vleerhond", "key %d", key);
    }

    void VleerhondApp::newMidiMessage(ofxMidiMessage& message)
    {
        switch (message.status)
        {
        case MIDI_TIME_CLOCK:
            MidiIO::sendTimeClock();
            if (Utils::intervalHit(TimeDivision::Sixteenth, data.time))
            {
                ofxOscMessage m;
                m.setAddress("/clock");
                m.addInt32Arg(this->data.time.tick);
                osc_sender.sendMessage( m );
            }
            handleClock(this->data);
            break;
        case MIDI_STOP:
            ofLogNotice("Vleerhond", "Stop!");
            MidiIO::sendStop();
            handleStop(this->data);
            stop_counter++;
            if (stop_counter > 8)
            {
                ::exit(0);
                // ofExit(0);
            }
            else //if (stop_counter > 1)
            {
                for (InstrumentBase* inst : data.getInstrumentPtrs())
                {
                    inst->getChannel()->allNotesOff();
                }
            }
            break;
        case MIDI_START:
        case MIDI_CONTINUE:
            MidiIO::sendContinue();
            MidiIO::sendStart();
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
            ofLogNotice("MIDIIN", "NoteOn(%d, %d, %d)", message.channel, message.pitch, message.velocity);
            switch (message.pitch)
            {
                case 48: // C3
                    data.minitaur.select(0);
                    break;
                case 64: // E4
                    data.minitaur.select(1);
                    break;
                case 80: // G#5
                    data.minitaur.select(2);
                    break;
                case 96: // C7
                    data.minitaur.select(3);
                    break;

                case 50:  // D3
                    data.neutron.select(0);
                    break;
                case 66:  // D3
                    data.neutron.select(1);
                    break;
                case 82:  // F#4
                    data.neutron.select(2);
                    break;
                case 98:  // A#5
                    data.neutron.select(3);
                    break;

                case 52:  // E3
                    data.harmony.setTonic();
                    break;
                case 53:  // F3
                    data.harmony.setType(HarmonyType::Const);
                    data.harmony.switchConstChord();
                    break;
                case 68:  // G#4
                    break;
                case 84:  // C6
                    data.addEvent(std::make_shared<ChangeHarmonyEvent>(HarmonyType::TonicLow, &data.harmony, &data.time));
                    break;
                case 85:  // C#6
                    data.addEvent(std::make_shared<ChangeHarmonyEvent>(HarmonyType::DominantLow, &data.harmony, &data.time));
                    break;
                case 100:  // E7
                    data.addEvent(std::make_shared<ChangeHarmonyEvent>(HarmonyType::TonicHigh, &data.harmony, &data.time));
                    break;
                case 101:  // F7
                    data.addEvent(std::make_shared<ChangeHarmonyEvent>(HarmonyType::DominantHigh, &data.harmony, &data.time));
                    break;
                default:
                    break;
            }
            break;
        case MIDI_NOTE_OFF:
            break;
        case MIDI_CONTROL_CHANGE:
            ofLogNotice("MIDIIN", "CC in: channel %d, control %d, value %d", message.channel, message.control, message.value);
//            handleControlChange(this->data, message.channel, message.control, message.value);
            if (message.channel != 10)
            {
                break;
            }
            switch(message.control)
            {
                case 43:
                    data.minitaur.setVariableDensity(message.value);
                    break;
                case 39:
                    data.minitaur.setVariablePitchOffset(message.value);
                    break;
                case 16:
                    data.neutron.setVariableDensity(message.value);
                    break;
                case 17:
                    data.neutron.setVariablePitchOffset(message.value);
                    break;
                default:
                    break;
            }
            break;
            //break;
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
