#pragma once

#include "instruments/instrument_base.h"
#include "patterns/gate_patterns.h"
#include "patterns/interval_pattern.h"
#include "patterns/modulators.h"

namespace Vleerhond {
class TanzbarMa : public InstrumentBase {
   protected:
    ModulationReceiver decay_mod;

   public:
    CvPatternAB ma_pattern;
    IntervalPattern interval_pattern;

    TanzbarMa(Modulators& modulators_ref, TimeStruct& time_ref);

    void randomize();

    bool play();
};
}  // namespace Vleerhond