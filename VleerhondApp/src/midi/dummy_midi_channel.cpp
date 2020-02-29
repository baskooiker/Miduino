#include "midi/dummy_midi_channel.h"

namespace Vleerhond
{
    void DummyMidiChannel::_send_note_on(const uint8_t pitch, const uint8_t velocity) {}
    void DummyMidiChannel::_send_note_off(const uint8_t pitch) {}
    DummyMidiChannel::DummyMidiChannel()
        : MidiChannel(-1, "")
    {}
}
