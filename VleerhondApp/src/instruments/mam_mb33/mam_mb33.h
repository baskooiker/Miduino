#pragma once

#include "instruments/instrument_rack.h"
#include "instruments/tonal_instruments/303_acid_bass.h"
#include "instruments/tonal_instruments/mono.h"

namespace Vleerhond {
class MamMb33 : public InstrumentRack {
   private:
    TB303Bass drop_bass;
    TB303Bass const_bass;
    TB303Bass acid_bass;
    Mono arp;

   public:
    MamMb33(HarmonyStruct& harmony, Modulators& modulators, TimeStruct& time);
    void randomize();
};
}  // namespace Vleerhond
