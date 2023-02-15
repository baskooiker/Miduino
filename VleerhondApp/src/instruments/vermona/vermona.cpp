#include "instruments/vermona/vermona.h"

namespace Vleerhond {
Vermona::Vermona(HarmonyStruct& harmony, TimeStruct& time)
    : InstrumentRack(time), poly(harmony, time), fugue(harmony, time) {
    instruments.push_back(&poly);
    instruments.push_back(&fugue);
}
}  // namespace Vleerhond