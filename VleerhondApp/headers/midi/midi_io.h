#pragma once

#include <map>
#include <vector>

#include "ofxMidiOut.h"
#include "ofxMidiIn.h"

namespace Vleerhond
{

    class MidiOut
    {
    public:
        ofxMidiOut port;
        int time_multiplier = 1;

        MidiOut(int time_multiplier = 1);
    };

    class MidiIO
    {
    private:
        static std::vector<ofxMidiIn> in_ports;
        static std::map<std::string, MidiOut> out_ports;

        static std::vector<MidiOut> getOutPorts(const std::string& port_name);

    public:
        static bool setMainInput(const std::string& port_name, ofxMidiListener* listener);
        static bool addOutput(const std::string& port_name, const int time_multiplier=1);

        static bool portAvailable(const std::string& port_name);
        static bool portsOpen();
        static void closeAll();

        static void sendTimeClock();

        static void send_note_on(const uint8_t pitch, const uint8_t velocity, const uint8_t channel, const std::string& port_name);
        static void send_note_off(const uint8_t pitch, const uint8_t channel, const std::string& port_name);
        static void send_cc(const uint8_t cc, const uint8_t value, const uint8_t channel, const std::string& port_name);
        static void sendBytes(std::vector<uint8_t>& bytes, const std::string& port_name);
        static void sendProgramChange(const uint8_t channel, const uint8_t program, const std::string& port_name);
    };
}
