#include "midi_io.hpp"

#include "ofLog.h"

namespace Vleerhond
{

    std::vector<ofxMidiOut> MidiIO::midi_out;
    std::vector<ofxMidiIn> MidiIO::midi_in;

    void MidiIO::send_note_on(const uint8_t pitch, const uint8_t velocity, const uint8_t channel)
    {
        for (ofxMidiOut& out : midi_out)
        {
            out.sendNoteOn(channel, pitch, velocity);
        }
    }

    void MidiIO::send_note_off(const uint8_t pitch, const uint8_t channel)
    {
        for (ofxMidiOut& out : midi_out)
        {
            out.sendNoteOff(channel, pitch);
        }
    }

    void MidiIO::send_cc(uint8_t cc, uint8_t value, uint8_t channel)
    {
        for (ofxMidiOut& out : midi_out)
        {
            out.sendControlChange(channel, cc, value);
        }
    }

    bool MidiIO::addMidiIn(std::string& name)
    {
        midi_in.push_back(ofxMidiIn());
        ofxMidiIn& port = midi_in[midi_in.size() - 1];

        uint8_t num_ports = port.getNumInPorts();

        int port_index = -1;
        for (int i = 0; i < num_ports; i++)
        {
            std::string port_name = port.getInPortName(i);
            int eq = port_name.find(name);
            ofLogNotice("midi in", "%s, num_ports = %d, eq = %d", port_name.c_str(), num_ports, eq);
            if (eq >= 0)
            {
                ofLogNotice("midi in", port_name);
                return port.openPort(i);
            }
        }
        return false;
    }

    bool MidiIO::addMidiOut(std::string& name)
    {
        midi_out.push_back(ofxMidiOut());
        ofxMidiOut& port = midi_out[midi_out.size() - 1];

        uint8_t num_ports = port.getNumOutPorts();

        int port_index = -1;
        for (int i = 0; i < num_ports; i++)
        {
            std::string port_name = port.getOutPortName(i);
            int eq = port_name.find(name);
            ofLogNotice("midi out", "%s, num_ports = %d, eq = %d", port_name.c_str(), num_ports, eq);
            if (eq >= 0)
            {
                ofLogNotice("midi out", port_name);
                return port.openPort(i);
            }
        }
        return false;
    }

    bool MidiIO::portsOpen()
    {
        for (ofxMidiIn& in : midi_in)
        {
            if (!in.isOpen())
            {
                return false;
            }
        }
        for (ofxMidiOut& out : midi_out)
        {
            if (!out.isOpen())
            {
                return false;
            }
        }
        return true;
    }

    std::vector<ofxMidiIn>& MidiIO::ins()
    {
        return midi_in;
    }

    std::vector<ofxMidiOut>& MidiIO::outs()
    {
        return midi_out;
    }
}
