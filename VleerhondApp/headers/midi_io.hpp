#pragma once

#include <vector>

#include "ofxMidiIn.h"
#include "ofxMidiOut.h"

namespace Vleerhond
{
    class MidiIO
    {
    private:
        static std::vector<ofxMidiOut> midi_out;
        // TODO: Make this a pointer to allow only one input
        static std::vector<ofxMidiIn> midi_in;

    public:
        static std::vector<ofxMidiIn>& ins();
        static std::vector<ofxMidiOut>& outs();

        static void send_note_on(const uint8_t pitch, const uint8_t velocity, const uint8_t channel);
        static void send_note_off(const uint8_t pitch, const uint8_t channel);
        static void send_cc(const uint8_t cc, const uint8_t value, const uint8_t channel);

        static bool addMidiIn(std::string& name);
        static bool addMidiOut(std::string& name);
        
        // TODO: Remove?
        static bool portsOpen();
    };
}