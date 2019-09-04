#pragma once

#include "parameters.h"
#include "mono_dub.h"
#include "modulators.h"
#include "harmony_struct.h"

namespace Vleerhond
{
    class VermonaMonoDub : public MonoDub
    {
    private:
        // TODO: use this
        ModulationReceiver velocity_mod;

    public:
        VermonaMonoDub(
            Mono& mono,
            HarmonyStruct& harmony,
            TimeStruct& time,
            Modulators& modulators) :
            MonoDub(mono, harmony, time),
            velocity_mod(modulators)
        {
            midi_channel.set_channel(MIDI_CHANNEL_MONO_2);
        }

        bool play()
        {
            if (this->lead.get_hit())
            {
                MidiIO::send_cc(MODULATION_WHEEL_CC, get_velocity(), MIDI_CHANNEL_MONO_2);
            }
            return MonoDub::play();
        }
    };
}
