#include "VleerhondApp.h"

#include "ofLog.h"
#include "ofxMidiOut.h"
#include "ofxMidiIn.h"

#include "callbacks.h"

ofxMidiOut midi_out_a;
ofxMidiOut midi_out_b;
ofxMidiIn midi_in;

void MidiIO::send_note_on(const uint8_t pitch, const uint8_t velocity, const uint8_t channel)
{
    //printf("send_note_on(%d, %d, %d)\n", pitch, velocity, channel);
    midi_out_a.sendNoteOn(channel, pitch, velocity);
    midi_out_b.sendNoteOn(channel, pitch, velocity);
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

//--------------------------------------------------------------
void VleerhondApp::setup(){
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

    if (out_b_index != -1)
    {
        midi_out_b.openPort(out_b_index);
        printf("Port '%d' is open = %d\n", out_b_index, midi_out_b.isOpen());
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

    if (!midi_in.isOpen() || !midi_out_a.isOpen() || !midi_out_b.isOpen())
    {
        ofExit();
    }

    // don't ignore sysex, timing, & active sense messages,
    // these are ignored by default
    midi_in.ignoreTypes(false, false, false);
    midi_in.addListener(this);
    midi_in.setVerbose(true);

    // Init app
    data.randomize_all();
}

//--------------------------------------------------------------
void VleerhondApp::update()
{
    data.process_events();
}

//--------------------------------------------------------------
void VleerhondApp::draw(){

}

//--------------------------------------------------------------
#define KEY_Q 113

void VleerhondApp::keyPressed(int key){
    switch (key)
    {
    case KEY_Q:
        ofExit();
        break;
    default:
        //printf("Unhandled key pressed: %3d\n", key);
        break;
    }
}

void VleerhondApp::newMidiMessage(ofxMidiMessage& message)
{
    switch (message.channel)
    {
    default:
        switch (message.status)
        {
        case MIDI_TIME_CLOCK:
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
}

//--------------------------------------------------------------
void VleerhondApp::keyReleased(int key){

}

//--------------------------------------------------------------
void VleerhondApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void VleerhondApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void VleerhondApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void VleerhondApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void VleerhondApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void VleerhondApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void VleerhondApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void VleerhondApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void VleerhondApp::dragEvent(ofDragInfo dragInfo){ 

}
