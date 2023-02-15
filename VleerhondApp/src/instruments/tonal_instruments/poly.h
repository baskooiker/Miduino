#pragma once

#include "instruments/instrument_base.h"
#include "patterns/gate_patterns.h"

namespace Vleerhond {
class Poly : public TonalInstrumentBase {
   protected:
    GatePatternAB gates_low;
    GatePatternAB gates;
    GatePatternAB tie_pattern;
    uint8_t pitch_offset;
    PolyType type;

   public:
    Poly(HarmonyStruct& harmony, TimeStruct& time);
    virtual void randomize();
    virtual bool play();
};
}  // namespace Vleerhond