#pragma once

#include "instrument_base.h"
#include "patterns/gate_patterns.h"
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

        struct 
        {
            uint8_t p_length_8 = 16;
            uint8_t p_length_16 = 16;
            uint8_t p_rand = 16;
            uint8_t p_drop = 16;
        } settings;

        Percussion(
            Modulators& modulators_ref,
            TimeStruct& time_ref,
            const uint8_t midi_channel
        );
        virtual void randomize();
        virtual bool play();
        virtual uint8_t get_velocity();
    };
}