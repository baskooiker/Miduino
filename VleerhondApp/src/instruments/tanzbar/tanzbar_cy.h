#pragma once

#include "core/time_struct.h"
#include "core/timing_structs.h"
#include "cymbal.h"
#include "instrument_base.h"
#include "midi/midi_io.h"
#include "patterns/gate_patterns.h"
#include "patterns/modulators.h"

namespace Vleerhond {
class TanzbarCy : public Cymbal {
   protected:
    ModulationReceiver cy_tune;

   public:
    TanzbarCy(Modulators& modulators_ref, TimeStruct& time_ref);

    void randomize();

    bool play();
};
}  // namespace Vleerhond
