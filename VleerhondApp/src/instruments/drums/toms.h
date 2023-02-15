#pragma once

#include "instrument_base.h"
#include "patterns/gate_patterns.h"
#include "patterns/interval_pattern.h"
#include "patterns/modulators.h"

namespace Vleerhond {
class Toms : public InstrumentBase {
   protected:
    MicroTimingStruct timing;

    ModulationReceiver tom_vel;

    CvPatternAB tom_pattern;

    std::vector<uint8_t> pitches;

   public:
    Toms(Modulators& modulators_ref, TimeStruct& time_ref);
    virtual void randomize();
    virtual bool play();
    virtual uint8_t getVelocity();
    virtual uint8_t getPitch(const TimeStruct& time);
};
}  // namespace Vleerhond