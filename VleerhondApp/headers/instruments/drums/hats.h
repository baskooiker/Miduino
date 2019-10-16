#pragma once

#include "instrument_base.h"
#include "gate_patterns.h"
#include "modulators.h"
#include "parameters.h"
#include "interval_pattern.h"

namespace Vleerhond
{
    class Hats : public InstrumentBase
    {
    protected:
        MicroTimingStruct timing;
        ModulationReceiver hats_vel;
        uint8_t pitch_closed;
        uint8_t pitch_open;

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
        virtual uint8_t get_velocity();
    };
}