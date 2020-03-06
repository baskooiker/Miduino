#pragma once

#include "drums/kick.h"
#include "midi/midi_io.h"

namespace Vleerhond
{
    class TanzbarLo : public Kick
    {
    protected:
        MicroTimingStruct bd_timing;
        ModulationReceiver bd2_pitch_mod;

    public:
        TanzbarLo(
            Modulators& modulators,
            TimeStruct& time);

        void randomize();

        void randomize_alternative();

        bool play();
    };
}