#pragma once

#include "core/defs.h"
#include "instrument_base.h"
#include "patterns/modulators.h"
#include "patterns/gate_patterns.h"
#include "snare.h"
#include "midi/midi_io.h"

namespace Vleerhond
{
    class TanzbarCp : public Snare
    {
    protected:
        ModulationReceiver cp_trig;
        ModulationReceiver filter_mod;

    public:
        TanzbarCp(
            Modulators& modulators_ref,
            TimeStruct& time_ref);

        void randomize();

        bool play();
    };
}