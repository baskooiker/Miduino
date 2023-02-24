#include "instruments/nord_drum/nord_drum_rack.h"

namespace Vleerhond {
NordDrumRack::NordDrumRack(
    HarmonyStruct& harmony, Modulators& modulators, TimeStruct& time)
    : InstrumentRack(time),
      percussion(modulators, time),
      kit(harmony, modulators, time) {
    instruments.push_back(&percussion);
    instruments.push_back(&kit);
}
}  // namespace Vleerhond