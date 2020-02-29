#pragma once

#include "instrument_base.h"
#include "patterns/gate_patterns.h"
#include "patterns/modulators.h"
#include "parameters.h"

namespace Vleerhond
{
    class Kick : public InstrumentBase
    {
    protected:
        MicroTimingStruct timing;
        uint8_t pitch;

    public:
        GatePatternAB bd_pattern;
        GatePatternAB ghost_notes;

        Kick(Modulators& modulators_ref, TimeStruct& time_ref);
        virtual void randomize();
        virtual bool play();
        virtual uint8_t get_velocity();

    protected:
        virtual void randomize_kick();
    };
}