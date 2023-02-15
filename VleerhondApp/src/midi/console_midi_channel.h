#pragma once

#include <set>

#include "midi/midi_channel.h"

namespace Vleerhond {
class ConsoleMidiChannel : public MidiChannel {
   private:
    std::map<uint8_t, int> notes;

   public:
    ConsoleMidiChannel(const std::string& port_name);

    virtual std::string getStorageString();

    virtual void _sendNoteOn(const uint8_t pitch, const uint8_t velocity);

    virtual void _sendNoteOff(const uint8_t pitch);

    virtual void print();

    int activeNoteCount();
};
}  // namespace Vleerhond