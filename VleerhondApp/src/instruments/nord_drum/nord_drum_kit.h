#pragma once

#include "instrument_group.h"
#include "patterns/modulators.h"

namespace Vleerhond {
class NordDrumKit : public InstrumentGroup {
   public:
    NordDrumKit(
        HarmonyStruct& harmony, Modulators& modulators, TimeStruct& time);
};
}  // namespace Vleerhond