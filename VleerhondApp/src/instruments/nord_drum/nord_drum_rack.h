#pragma once

#include "instrument_group.h"
#include "instruments/nord_drum/nord_drum_kit.h"
#include "instruments/nord_drum/nord_drum_percussion.h"
#include "patterns/modulators.h"

namespace Vleerhond {
class NordDrumRack : public InstrumentGroup {
   public:
    NordDrumPercussion percussion;
    NordDrumKit kit;

    NordDrumRack(
        HarmonyStruct& harmony, Modulators& modulators, TimeStruct& time);
};
}  // namespace Vleerhond