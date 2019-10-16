#pragma once

#include "instrument_base.h"
#include "gate_patterns.h"
#include "modulators.h"
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
        GatePatternAB accents;

        Kick(
            Modulators& modulators_ref,
            TimeStruct& time_ref
        );
        virtual void randomize();
        virtual bool play();
        virtual uint8_t get_velocity();

    protected:
        virtual void randomize_kick();
    };
}