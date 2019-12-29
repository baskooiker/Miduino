#pragma once

#include "parameters.h"
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
                MidiIO::send_cc(MODULATION_WHEEL_CC, get_velocity(), MIDI_CHANNEL_MONO);
            }
            return Mono::play();
        }
    };
}

