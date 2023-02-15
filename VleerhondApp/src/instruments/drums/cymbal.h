#pragma once

#include "instrument_base.h"
#include "patterns/gate_patterns.h"
#include "patterns/modulators.h"

namespace Vleerhond {
class Cymbal : public InstrumentBase {
   protected:
    MicroTimingStruct timing;
    ModulationReceiver cy_vel;
    uint8_t pitch;
    struct {
        uint8_t min_velocity = 64;
        uint8_t max_velocity = 127;
    } settings;

   public:
    GatePatternAB cy_pattern;

    Cymbal(Modulators& modulators_ref, TimeStruct& time_ref);
    virtual void randomize();
    virtual bool play();
    virtual uint8_t getVelocity();
};
}  // namespace Vleerhond