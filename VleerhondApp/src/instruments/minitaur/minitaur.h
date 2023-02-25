#pragma once

#include "instruments/instrument_rack.h"
#include "instruments/minitaur/moog_bass.h"
#include "instruments/minitaur/moog_long_bass.h"

namespace Vleerhond {

class Minitaur : public InstrumentRack {
   public:
    MoogBass bass_root;
    MoogBass bass_funk;
    MoogLongBass bass_long;

    Minitaur(HarmonyStruct& harmony, Modulators& modulators, TimeStruct& time);

    void randomize();
};
}  // namespace Vleerhond
