#pragma once

#include "instruments/instrument_base.h"
#include "patterns/gate_patterns.h"
#include "patterns/modulators.h"
#include "patterns/interval_pattern.h"

namespace Vleerhond
{
    class Hats : public InstrumentBase
    {
    protected:
        MicroTimingStruct timing;
        ModulationReceiver hats_vel;
        uint8_t pitch_closed;
        uint8_t pitch_open;

        struct {
            uint8_t p_off = 16;
            uint8_t p_euclid = 16;
            uint8_t p_drop = 16;
            uint8_t velocity_range = 50;
            uint8_t velocity_offset = 32;
            bool choke_open = true;
        } settings;

    public:
        GatePatternAB hh_pattern;
        GatePatternAB oh_pattern;

        IntervalPattern hat_int_pattern;
        CvPatternAB hat_velocity;

        HatClosedStyle hat_closed_style;

        Hats(
            Modulators& modulators_ref,
            TimeStruct& time_ref
        );
        virtual void randomize();
        virtual void randomize_seq();
        virtual bool play_hats_closed();
        virtual bool play_hats_open();
        virtual bool play();
        virtual uint8_t getVelocity();
    };
}