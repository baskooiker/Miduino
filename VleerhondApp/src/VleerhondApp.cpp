#include "VleerhondApp.h"

#include "ofLog.h"
#include "ofxMidiOut.h"
#include "ofxMidiIn.h"

#include "callbacks.h"
#include "defs.h"
#include "midi_io.h"

ofxMidiOut midi_out_a;
ofxMidiOut midi_out_b;
ofxMidiIn midi_in;

const char MODULE[] = "VleerhondApp";

bool virtual_ports;

void MidiIO::send_note_on(const uint8_t pitch, const uint8_t velocity, const uint8_t channel)
{
    midi_out_a.sendNoteOn(channel, pitch, velocity);
    midi_out_b.sendNoteOn(channel, pitch, velocity);
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
}

void MidiIO::send_cc(uint8_t cc, uint8_t value, uint8_t channel)
{
    midi_out_a.sendControlChange(channel, cc, value);
    midi_out_b.sendControlChange(channel, cc, value);
}

void initialize_midi_ports()
{
    ofSetBackgroundColor(0);

    virtual_ports = false;
    uint8_t num_ports = midi_out_a.getNumOutPorts();
    printf("%d\n", num_ports);

    std::string midi_a_name = "MIDISPORT 2x2 Anniversary Out A";
    std::string midi_b_name = "MIDISPORT 2x2 Anniversary Out B";

    int out_a_index = -1;
    int out_b_index = -1;
    for (int i = 0; i < num_ports; i++)
    {
        std::string port_name = midi_out_a.getOutPortName(i);
        if (port_name.find(midi_a_name) == 0)
            out_a_index = i;
        if (port_name.find(midi_b_name) == 0)
            out_b_index = i;
        printf("  %d: %s\n", i, port_name.c_str());
    }

    if (out_a_index != -1)
    {
        midi_out_a.openPort(out_a_index);
        printf("Port '%d' is open = %d\n", out_a_index, midi_out_a.isOpen());
    }
    else
    {
        midi_out_a.openVirtualPort(midi_a_name);
        printf("Virtual port '%s' is open = %d\n", midi_a_name.c_str(), midi_out_a.isOpen());
        virtual_ports = true;
    }

    if (out_b_index != -1)
    {
        midi_out_b.openPort(out_b_index);
        printf("Port '%d' is open = %d\n", out_b_index, midi_out_b.isOpen());
    }
    else
    {
        midi_out_b.openVirtualPort(midi_b_name);
        printf("Virtual port '%s' is open = %d\n", midi_b_name.c_str(), midi_out_b.isOpen());
        virtual_ports = true;
    }

    uint8_t num_in_ports = midi_in.getNumInPorts();
    printf("%d\n", num_in_ports);
    std::string midi_in_name = "MIDISPORT 2x2 Anniversary In A";

    int in_index = -1;
    for (int i = 0; i < num_in_ports; i++)
    {
        std::string in_port_name = midi_in.getInPortName(i);
        if (in_port_name.find(midi_in_name) == 0 && in_index == -1)
            in_index = i;
        printf("  %d: %s\n", i, in_port_name.c_str());
    }
    if (in_index != -1)
    {
        midi_in.openPort(in_index);
        printf("Port '%d' is open = %d\n", in_index, midi_in.isOpen());
    }
    else
    {
        midi_in.openVirtualPort(midi_in_name);
        printf("Virtual port '%s' is open = %d\n", midi_in_name.c_str(), midi_in.isOpen());
        virtual_ports = true;
    }

    if (virtual_ports)
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

    ofAddListener(this->bpm.beatEvent, this, &VleerhondApp::play);
    bpm.setBpm(120 * 24);
    bpm.setBeatPerBar(4);

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
    if (virtual_ports 
        || !midi_in.isOpen() 
        || !midi_out_a.isOpen() 
        || !midi_out_b.isOpen())
    {
        ofSleepMillis(500);
        initialize_midi_ports();
    }
    else
    {
        data.process_events();
    }
}

void VleerhondApp::draw(){}

void VleerhondApp::keyPressed(int key){
    switch (key)
    {
    case 'q':
        ofExit();
        break;
    case 'p':
        ofLogNotice(MODULE, "Play!");
        bpm.start();
        break;
    case 's':
        ofLogNotice(MODULE, "Stop.");
        bpm.stop();
        data.time.state = PlayState::Stopped;
        break;
    case 'r':
        ofLogNotice(MODULE, "Randomize");
        data.randomize_all();
        break;
    case 'i':
        initialize_midi_ports();
        break;
    case 'c' :
        break;
    default:
        //ofLogNotice(MODULE, "Unhandled key pressed: %3d\n", key);
        break;
    }
}

void VleerhondApp::newMidiMessage(ofxMidiMessage& message)
{
    switch (message.status)
    {
    case MIDI_TIME_CLOCK:
        bpm.stop();
        handleClock(this->data);
        break;
    case MIDI_STOP:
        handleStop(this->data);
        break;
    case MIDI_START:
        break;
    case MIDI_NOTE_ON:
        handleNoteOn(this->data, message.channel, message.pitch, message.velocity);
        break;
    case MIDI_NOTE_OFF:
        handleNoteOff(this->data, message.channel, message.pitch, message.velocity);
        break;
    case MIDI_CONTROL_CHANGE:
        handleControlChange(this->data, message.channel, message.control, message.value);
        break;
    default:
        //printf(
        //    "Unknown MIDI message: channel %2d\n", message.channel);
        break;
    }
}

void VleerhondApp::keyReleased(int key){}
void VleerhondApp::mouseMoved(int x, int y ){}
void VleerhondApp::mouseDragged(int x, int y, int button){}
void VleerhondApp::mousePressed(int x, int y, int button){}
void VleerhondApp::mouseReleased(int x, int y, int button){}
void VleerhondApp::mouseEntered(int x, int y){}
void VleerhondApp::mouseExited(int x, int y){}
void VleerhondApp::windowResized(int w, int h){}
void VleerhondApp::gotMessage(ofMessage msg){}
void VleerhondApp::dragEvent(ofDragInfo dragInfo){ }

void VleerhondApp::exit()
{
    bpm.stop();
}
