#pragma once

#include "instrument_base.h"
#include "gate_patterns.h"
#include "modulators.h"
#include "parameters.h"

namespace Vleerhond
{
    class Percussion : public InstrumentBase
    {
    protected:
        MicroTimingStruct timing;
        ModulationReceiver velocity_mod;
        GatePatternAB pattern;

        uint8_t pitch;

    public:

        Percussion(
            Modulators& modulators_ref,
            TimeStruct& time_ref
        );
        void randomize();
        bool play();
        virtual uint8_t get_velocity();
    };
}