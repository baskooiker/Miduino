#pragma once

#include "instrument_base.h"
#include "core/timing_structs.h"
#include "core/time_struct.h"
#include "patterns/modulators.h"
#include "patterns/gate_patterns.h"
#include "cymbal.h"
#include "midi/midi_io.h"

namespace Vleerhond
{
    class TanzbarCy : public Cymbal
    {
    protected:
        ModulationReceiver cy_tune;

    public:
        TanzbarCy(
            Modulators& modulators_ref,
            TimeStruct& time_ref);

        void randomize();

        bool play();
    };
}
