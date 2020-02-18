#include "midi/midi_io.h"

#include "ofLog.h"

#include "core/defs.h"

namespace Vleerhond
{
    std::vector<ofxMidiIn> MidiIO::in_ports;
    std::map<std::string, MidiOut> MidiIO::out_ports;

    bool nameMatches(const std::string& port_name, const std::string& target_name)
    {
        return port_name.find(target_name) != -1;
    }

    bool MidiIO::setMainInput(const std::string& target_port_name, ofxMidiListener* listener)
    {
        in_ports.push_back(ofxMidiIn());

        ofxMidiIn& in = in_ports.back();

        ofLogNotice("MIDI", "finding port name: %s", target_port_name.c_str());
        std::vector<std::string> port_names = in.getInPortList();
        for (const std::string& name: port_names)
        {
            ofLogNotice("MIDI", "In port: %s", name);
        }

        for (int i = 0; i < port_names.size(); i++)
        {
            if (nameMatches(port_names[i], target_port_name))
            {
                if (in.openPort(i))
                {
                    // don't ignore sysex, timing, & active sense messages,
                    // these are ignored by default
                    in.ignoreTypes(false, false, false);
                    in.setVerbose(true);
                    in.addListener(listener);
                    ofLogNotice("", "input succesfully initialized: %s", in.getInPortName(i));
                    return true;
                }
            }
        }
        ofLogNotice("input not initialized!");
        return false;
    }

    bool MidiIO::addOutput(const std::string& target_port_name, const int time_multiplier)
    {
        out_ports.insert(std::make_pair(target_port_name, MidiOut(time_multiplier)));

        MidiOut& pair = out_ports[target_port_name];
        ofxMidiOut& out = pair.port;

        for (const std::string name : out.getOutPortList())
        {
            ofLogNotice("MIDI", "Out port: %s", name.c_str());
        }

        for (int i = 0; i < out.getNumOutPorts(); i++)
        {
            if (nameMatches(out.getOutPortName(i), target_port_name))
            {
                if (out.openPort(i))
                {
                    return true;
                }
            }
        }
        return false;
    }

    bool MidiIO::portAvailable(const std::string & target_port_name)
    {
        ofxMidiIn in;

        for (const std::string& name : in.getInPortList())
        {
            if (nameMatches(name, target_port_name))
            {
                return true;
            }
        }
        return false;
    }

    bool MidiIO::portsOpen()
    {
        for (auto& in : in_ports)
        {
            if (!in.isOpen())
            {
                return false;
            }
        }
        for (auto& out : out_ports)
        {
            if (!out.second.port.isOpen())
            {
                return false;
            }
        }

        return true;
    }

    void MidiIO::closeAll()
    {
        for (auto& in : in_ports)
        {
            in.closePort();
        }
        for (auto& out : out_ports)
        {
            out.second.port.closePort();
        }
    }

    void MidiIO::sendTimeClock()
    {
        for (auto& out : out_ports)
        {
            for (int i = 0; i < out.second.time_multiplier; i++)
            {
                out.second.port.sendMidiByte(0xF8);
            }
        }
    }

    void MidiIO::send_note_on(const uint8_t pitch, const uint8_t velocity, const uint8_t channel)
    {
        for (auto& out : out_ports)
        {
            out.second.port.sendNoteOn(channel, pitch, velocity);
        }
    }

    void MidiIO::send_note_off(const uint8_t pitch, const uint8_t channel)
    {
        for (auto& out : out_ports)
        {
            out.second.port.sendNoteOff(channel, pitch);
        }
    }

    void MidiIO::send_cc(uint8_t cc, uint8_t value, uint8_t channel)
    {
        for (auto& out: out_ports)
        {
            out.second.port.sendControlChange(channel, cc, value);
        }
    }

    void MidiIO::sendBytes(std::vector<uint8_t>& bytes)
    {
        for (auto& out : out_ports)
        {
            out.second.port.sendMidiBytes(bytes);
        }
    }

    void MidiIO::sendProgramChange(const uint8_t channel, const uint8_t program)
    {
        for (auto& out : out_ports)
        {
            out.second.port.sendProgramChange(channel, program);
        }
    }

    inline MidiOut::MidiOut(int time_multiplier)
        : time_multiplier(time_multiplier)
    {
    }

}
