#pragma once

#include "instrument_base.h"
#include "gate_patterns.h"
#include "modulators.h"
#include "parameters.h"
#include "interval_pattern.h"

namespace Vleerhond
{
    class Toms : public InstrumentBase
    {
    protected:
        MicroTimingStruct timing;

        ModulationReceiver tom_vel;

        CvPatternAB tom_pattern;

        std::vector<uint8_t> pitches;

    public:

        Toms(
            Modulators& modulators_ref,
            TimeStruct& time_ref,
            const uint8_t midi_channel
        );
        virtual void randomize();
        virtual bool play();
        virtual uint8_t get_velocity();
        virtual uint8_t get_pitch(const TimeStruct& time);
    };
}