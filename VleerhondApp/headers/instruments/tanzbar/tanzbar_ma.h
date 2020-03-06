#pragma once

#include "instruments/instrument_base.h"
#include "patterns/gate_patterns.h"
#include "patterns/interval_pattern.h"
#include "patterns/modulators.h"

namespace Vleerhond
{
    enum class PatternType
    {
        Interval,
        Gate
    };

    class RhythmPattern
    {
    public:
        GatePatternAB gate_pattern;
        IntervalPattern interval_pattern;
        PatternType type;

        bool value(const TimeStruct& time);

        void randomize_type();
    };

    class TanzbarMa : public InstrumentBase
    {
    protected:
        ModulationReceiver decay_mod;

    public:
        CvPatternAB ma_pattern;
        IntervalPattern interval_pattern;

        TanzbarMa(
            Modulators& modulators_ref,
            TimeStruct& time_ref);

        void randomize();

        bool play();
    };
}