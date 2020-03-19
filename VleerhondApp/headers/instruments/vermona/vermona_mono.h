#pragma once

#include "mono.h"
#include "patterns/modulators.h"
#include "harmony/harmony_struct.h"

namespace Vleerhond
{
    class VermonaMono : public Mono
    {
    public:
        VermonaMono(
            HarmonyStruct& harmony,
            Modulators& modulators,
            TimeStruct& time) :
            Mono(harmony, modulators, time, MIDI_CHANNEL_MONO)
        {
        }

        bool play()
        {
            if (this->get_hit())
            {
                getChannel()->sendCC(MODULATION_WHEEL_CC, getVelocity());
            }
            return Mono::play();
        }
    };
}

