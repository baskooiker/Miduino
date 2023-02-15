#include "instruments/vermona/vermona_poly.h"

namespace Vleerhond {
VermonaPoly::VermonaPoly(HarmonyStruct& harmony, TimeStruct& time)
    : Poly(harmony, time) {
    this->setVariablePitchOffset(64);
}
}  // namespace Vleerhond
