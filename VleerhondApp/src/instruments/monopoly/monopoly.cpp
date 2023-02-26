#include "monopoly.h"

namespace Vleerhond {
Monopoly::Monopoly(
    HarmonyStruct& harmony, Modulators& modulators, TimeStruct& time)
    : InstrumentRack(time),
      long_bass(harmony, modulators, time),
      arp(harmony, modulators, time),
      acid(harmony, time),
      poly(harmony, time) {
    instruments.push_back(&long_bass);
    instruments.push_back(&arp);
    instruments.push_back(&acid);
    instruments.push_back(&poly);
}
}  // namespace Vleerhond
