#pragma once

#include "core/defs.h"
#include "instrument_base.h"
#include "midi/midi_io.h"
#include "patterns/gate_patterns.h"
#include "patterns/modulators.h"
#include "toms.h"

namespace Vleerhond {
class TanzbarTom : public Toms {
    ModulationReceiver low_pitch;
    ModulationReceiver mid_pitch;
    ModulationReceiver high_pitch;

   public:
    TanzbarTom(Modulators& modulators, TimeStruct& time);

    void randomize();

    bool play();
};
}  // namespace Vleerhond