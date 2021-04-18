#pragma once

#include "core/defs.h"
#include "instruments/instrument_base.h"
#include "patterns/modulators.h"
#include "patterns/gate_patterns.h"
#include "instruments/drums/percussion.h"
#include "midi/midi_io.h"

namespace Vleerhond
{
    class TanzbarRs : public Percussion
    {
    protected:
        ModulationReceiver rs_tune;

    public:
        TanzbarRs(
            Modulators& modulators_ref,
            TimeStruct& time_ref);

        void randomize();

        bool play();
    };
}