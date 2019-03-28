#include "VleerhondApp.h"

#include "ofLog.h"
#include "ofxMidiOut.h"
#include "ofxMidiIn.h"

#include "Rompler909.h"

#include "callbacks.h"

ofxMidiOut midi_out_a;
ofxMidiOut midi_out_b;
ofxMidiIn midi_in;

Rompler909 rompler909;

bool virtual_ports;
uint64_t time_0;
uint64_t counter;

void MidiIO::send_note_on(const uint8_t pitch, const uint8_t velocity, const uint8_t channel)
{
    midi_out_a.sendNoteOn(channel, pitch, velocity);
    midi_out_b.sendNoteOn(channel, pitch, velocity);
    switch (channel)
    {
    case MIDI_CHANNEL_TANZBAR:
        rompler909.play(pitch, velocity);
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
}

//--------------------------------------------------------------

void VleerhondApp::setup(){
    time_0 = 0;
    counter = 0;

    initialize_midi_ports();

    if (virtual_ports)
    {
        rompler909.load_sounds();
    }

    if (!midi_in.isOpen() || !midi_out_a.isOpen() || !midi_out_b.isOpen())
    {
        printf("One or more ports failed to open. EXIT!\n");
        ofExit();
    }

    // don't ignore sysex, timing, & active sense messages,
    // these are ignored by default
    midi_in.ignoreTypes(false, false, false);
    midi_in.addListener(this);
    midi_in.setVerbose(true);

    ofAddListener(this->bpm.beatEvent, this, &VleerhondApp::play);
    bpm.setBpm(120 * 24);
    bpm.start();

    // Init app
    data.randomize_all();
}

void VleerhondApp::play()
{
    data.process_events();
    data.stop_notes_all_instruments();
    data.play_all();
    data.time.tick++;
}

//--------------------------------------------------------------
void VleerhondApp::update()
{
    //data.process_events();
    //// 20000 us = 20 ms -> 120 bpm
    //uint64_t new_count = (ofGetSystemTimeMicros() - time_0) / 20000;
    //if (new_count > counter)
    //{
    //    data.stop_notes_all_instruments();
    //    data.play_all();
    //    counter = new_count;
    //    data.time.tick++;
    //}
}

//--------------------------------------------------------------
void VleerhondApp::draw(){

}

//--------------------------------------------------------------

void VleerhondApp::keyPressed(int key){
    switch (key)
    {
    case 'q':
        ofExit();
        break;
    case 'i':
        rompler909.play(NOTE_TANZBAR_BD1, 64);
        break;
    case 'p':
        bpm.start();
        break;
    case 's':
        bpm.stop();
        break;
    default:
        printf("Unhandled key pressed: %3d\n", key);
        break;
    }
}

void VleerhondApp::newMidiMessage(ofxMidiMessage& message)
{
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

void VleerhondApp::exit()
{
    printf("Calling Exit\n");
    bpm.stop();
}
