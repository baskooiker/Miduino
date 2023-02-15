#pragma once

#include "midi/midi_channel.h"

namespace Vleerhond {
class DummyMidiChannel : public MidiChannel {
   private:
    virtual void _sendNoteOn(const uint8_t pitch, const uint8_t velocity);
    virtual void _sendNoteOff(const uint8_t pitch);

   public:
    DummyMidiChannel();
};
}  // namespace Vleerhond
