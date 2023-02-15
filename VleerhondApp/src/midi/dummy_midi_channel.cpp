#include "midi/dummy_midi_channel.h"

namespace Vleerhond {
void DummyMidiChannel::_sendNoteOn(
    const uint8_t pitch, const uint8_t velocity) {}
void DummyMidiChannel::_sendNoteOff(const uint8_t pitch) {}
DummyMidiChannel::DummyMidiChannel() : MidiChannel(-1, "") {}
}  // namespace Vleerhond
