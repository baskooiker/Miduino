#pragma once

#include "mono_dub.h"
#include "patterns/modulators.h"
#include "harmony/harmony_struct.h"

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
            MonoDub(mono, harmony, time, MIDI_CHANNEL_MONO_2),
            velocity_mod(modulators)
        {
        }

        void randomize()
        {
            velocity_mod.randomize(Rand::randui8(128), 0, .5);
            MonoDub::randomize();
        }

        uint8_t get_velocity()
        {
            uint8_t value = 0;
            velocity_mod.value(time, value);
            return value;
        }

        bool play()
        {
            if (this->lead.get_hit())
            {
                getChannel()->sendCC(MODULATION_WHEEL_CC, get_velocity());
            }
            return MonoDub::play();
        }
    };
}

