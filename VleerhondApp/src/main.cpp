#include "ofMain.h"
#include "ofApp.h"

uint32_t millis()
{
    return ofGetSystemTimeMillis();
}

#include "ofLog.h"
#include "ofxMidiOut.h"
#include "ofxMidiIn.h"

ofxMidiOut midi_out;
ofxMidiIn midi_in;

void MidiIO::send_note_on(const uint8_t pitch, const uint8_t velocity, const uint8_t channel)
{
    //printf("send_note_on(%d, %d, %d)\n", pitch, velocity, channel);
    midi_out.sendNoteOn(channel, pitch, velocity);
}

void MidiIO::send_note_off(const uint8_t pitch, const uint8_t channel)
{
    midi_out.sendNoteOff(channel, pitch);
}

void MidiIO::send_cc(uint8_t cc, uint8_t value, uint8_t channel)
{
    midi_out.sendControlChange(channel, cc, value);
}


class MidiListener : public ofxMidiListener {
public:
    void newMidiMessage(ofxMidiMessage& message)
    {
        printf("Message received\n");
    }
};

//========================================================================
int main( ){
    uint8_t num_ports = midi_out.getNumOutPorts();
    printf("%d\n", num_ports);

    std::string midi_name = "Arturia";

    int index = -1;
    for (int i = 0; i < num_ports; i++)
    {
        std::string port_name = midi_out.getOutPortName(i);
        if (port_name.find(midi_name) == 0 && index == -1)
            index = i;
        printf("  %d: %s\n", i, port_name.c_str());
    }
    if (index != -1)
    {
        midi_out.openPort(index);
        printf("Port '%d' is open = %d\n", index, midi_out.isOpen());
    }

    uint8_t num_in_ports = midi_in.getNumInPorts();
    printf("%d\n", num_in_ports);
    std::string midi_in_name = "Arturia";
    //std::string midi_in_name = "MIDIIN2";
    //std::string midi_in_name = "Rocket";

    int in_index = -1;
    for (int i = 0; i < num_in_ports; i++)
    {
        std::string in_port_name = midi_in.getInPortName(i);
        if (in_port_name.find(midi_in_name) == 0 && in_index == -1)
            in_index = i;
        printf("  %d: %s\n", i, in_port_name.c_str());
    }
    if (index != -1)
    { 
        midi_in.openPort(in_index);
        printf("Port '%d' is open = %d\n", in_index, midi_in.isOpen());
    }

    if (!midi_in.isOpen() || !midi_out.isOpen())
    {
        return 1;
    }

    MidiListener sync_listener;
    midi_in.addListener(&sync_listener);

    if (false)
    {
        ofSetupOpenGL(1024, 768, OF_WINDOW);			// <-------- setup the GL context
        // this kicks off the running of my app
        // can be OF_WINDOW or OF_FULLSCREEN
        // pass in width and height too:
        ofRunApp(new ofApp());
    }
    else
    {
        ofApp app;
        app.setup();
        while (1) 
        {
            app.update();
        }
    }
}
