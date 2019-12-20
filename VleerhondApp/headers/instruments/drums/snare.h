#pragma once

#include "instrument_base.h"
#include "patterns/gate_patterns.h"
#include "modulators.h"
#include "parameters.h"

namespace Vleerhond
{

    struct SnareSettings
    {
        uint8_t p_coef = 16;
        uint8_t p_off = 32;
        uint8_t p_rand = 16;
        float p_rand_min = .75;
        float p_rand_max = 1.0;
        uint8_t p_length_8 = 16;
        uint8_t p_length_16 = 16;
    };


    class Snare : public InstrumentBase
    {
    protected:
        GatePatternAB pattern;
        MicroTimingStruct timing;

        uint8_t pitch;

    public:
        bool snare_roll;

        SnareSettings settings;

        Snare(
            Modulators& modulators_ref,
            TimeStruct& time_ref,
            const uint8_t midi_channel
        );
        virtual void randomize();
        virtual void play_roll(const TimeStruct& time);
        virtual bool play();
        virtual uint8_t get_velocity();
        virtual void printPattern();
    };
}