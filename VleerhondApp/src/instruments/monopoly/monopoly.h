#pragma once

#include "instruments/instrument_rack.h"
#include "instruments/tonal_instruments/long_bass.h"
#include "instruments/tonal_instruments/mono.h"
#include "patterns/modulators.h"

namespace Vleerhond {
class Monopoly : public InstrumentRack {
   public:
    LongBass long_bass;
    Mono arp;
    Monopoly(HarmonyStruct& harmony, Modulators& modulators, TimeStruct& time);
};
}  // namespace Vleerhond