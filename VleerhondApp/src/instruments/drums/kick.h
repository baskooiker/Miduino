#pragma once

#include "instrument_base.h"
#include "patterns/gate_patterns.h"
#include "patterns/modulators.h"

namespace Vleerhond {

struct KickSettings {
    bool play_ghost_notes = false;
    uint8_t min_velocity = 32;
    uint8_t max_velocity = 127;
};

class Kick : public InstrumentBase {
   protected:
    MicroTimingStruct timing;
    uint8_t pitch;

   public:
    KickSettings settings;

    GatePatternAB bd_pattern;
    GatePatternAB ghost_notes;

    Kick(Modulators& modulators_ref, TimeStruct& time_ref, uint8_t pitch);
    virtual void randomize();
    virtual bool play();
    virtual uint8_t getVelocity();

   protected:
    virtual void randomize_kick();
};
}  // namespace Vleerhond
