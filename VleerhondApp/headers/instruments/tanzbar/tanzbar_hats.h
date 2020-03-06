#pragma once

#include "instruments/drums/hats.h"
#include "midi/midi_io.h"

namespace Vleerhond
{
    class TanzbarHats : public Hats
    {
    protected:
        ModulationReceiver tune_mod;
        ModulationReceiver oh_dec;

    public:
        GatePatternAB hh_pattern;
        GatePatternAB oh_pattern;

        TanzbarHats(
            Modulators& modulators,
            TimeStruct& time);

        void randomize();

        bool play();
    };
}