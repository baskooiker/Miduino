#pragma once

#include "midi/midi_channel.h"

namespace Vleerhond
{
class DummyMidiChannel : public MidiChannel
{
private:
    virtual void _send_note_on(const uint8_t pitch, const uint8_t velocity);
    virtual void _send_note_off(const uint8_t pitch);

public:
    DummyMidiChannel();
};
}
