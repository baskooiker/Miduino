#pragma once

#include "parameters.h"
#include "mono.h"
#include "modulators.h"
#include "harmony_struct.h"

namespace Vleerhond
{
    class VermonaMono : public Mono
    {
    private:
        ModulationReceiver velocity_mod;

    public:
        VermonaMono(
            HarmonyStruct& harmony,
            Modulators& modulators,
            TimeStruct& time) :
            Mono(harmony, time),
            velocity_mod(modulators)
        {
            midi_channel.set_channel(MIDI_CHANNEL_MONO);
        }

        void randomize()
        {
            Mono::randomize();
            velocity_mod.randomize(128, 0, .5);
        }

        uint8_t get_velocity()
        {
            uint8_t value = 0;
            this->velocity_mod.value(time, value);
            return value;
        }

        bool play()
        {
            if (this->get_hit())
            {
                MidiIO::send_cc(MODULATION_WHEEL_CC, get_velocity(), MIDI_CHANNEL_MONO);
            }
            return Mono::play();
        }
    };
}

