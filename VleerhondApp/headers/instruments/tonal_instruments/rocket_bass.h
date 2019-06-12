#pragma once

#include "bass.h"

namespace Vleerhond
{
    class RocketBass : public Bass
    {
    protected:
        ModulationReceiver vel_mod;

    public:
        RocketBass(
            Modulators& modulators,
            HarmonyStruct& harmony,
            TimeStruct& time) :
            Bass(harmony, time),
            vel_mod(modulators)
        {
            midi_channel.set_channel(MIDI_CHANNEL_MINITAUR);
        }

        virtual void randomize()
        {
            Bass::randomize();
            uint8_t range = Rand::randui8(32);
            vel_mod.randomize(range, 64);
        }

        virtual uint8_t get_velocity()
        {
            uint8_t value = 100;
            vel_mod.value(time, value);
            return value;
        }
    };
}
