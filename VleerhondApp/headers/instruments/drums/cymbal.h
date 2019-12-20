#pragma once

#include "instrument_base.h"
#include "patterns/gate_patterns.h"
#include "modulators.h"
#include "parameters.h"

namespace Vleerhond
{
    class Cymbal : public InstrumentBase
    {
    protected:
        MicroTimingStruct timing;
        ModulationReceiver cy_vel;
        uint8_t pitch;

    public:
        GatePatternAB cy_pattern;

        Cymbal(
            Modulators& modulators_ref,
            TimeStruct& time_ref,
            const uint8_t midi_channel
        );
        virtual void randomize();
        virtual bool play();
        virtual uint8_t get_velocity();
    };
}