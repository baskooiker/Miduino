#pragma once

#include "instrument_base.h"
#include "gate_patterns.h"
#include "modulators.h"
#include "parameters.h"

namespace Vleerhond
{
    class Snare : public InstrumentBase
    {
    protected:
        GatePatternAB pattern;
        MicroTimingStruct timing;

        uint8_t pitch;

    public:
        bool snare_roll;

        Snare(
            Modulators& modulators_ref,
            TimeStruct& time_ref
        );
        virtual void randomize();
        virtual void play_roll(const TimeStruct& time);
        virtual bool play();
        virtual uint8_t get_velocity();
    };
}