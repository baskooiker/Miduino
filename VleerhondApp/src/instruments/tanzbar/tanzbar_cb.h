#pragma once

#include "core/defs.h"
#include "instrument_base.h"
#include "midi/midi_io.h"
#include "patterns/gate_patterns.h"
#include "patterns/modulators.h"
#include "percussion.h"

namespace Vleerhond {
class TanzbarCb : public Percussion {
   protected:
    ModulationReceiver cb_tune;

   public:
    TanzbarCb(Modulators& modulators_ref, TimeStruct& time_ref);

    void randomize();

    bool play();
};
}  // namespace Vleerhond