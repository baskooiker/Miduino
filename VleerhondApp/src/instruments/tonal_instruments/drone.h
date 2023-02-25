#pragma once

#include "instrument_base.h"

namespace Vleerhond {
class Drone : public TonalInstrumentBase {
    GatePatternAB gate_pattern;
    CvPatternAB interval_pattern;

   public:
    uint8_t variable_pitch_range;

    Drone(HarmonyStruct& harmony_ref, TimeStruct& time_ref);

    void randomize();

    bool play();
};
}  // namespace Vleerhond