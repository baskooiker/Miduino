#include "instruments/nord_drum/nord_drum_kit.h"

namespace Vleerhond {
NordDrumKit::NordDrumKit(
    HarmonyStruct& harmony, Modulators& modulators, TimeStruct& time)
    : InstrumentGroup(harmony, time) {}
}  // namespace Vleerhond