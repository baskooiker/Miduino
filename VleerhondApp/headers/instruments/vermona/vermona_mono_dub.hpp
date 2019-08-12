#pragma once

#include "parameters.hpp"
#include "mono_dub.hpp"
#include "modulators.hpp"
#include "harmony_struct.hpp"

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

