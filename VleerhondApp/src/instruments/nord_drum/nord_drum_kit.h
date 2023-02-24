#pragma once

#include "instrument_group.h"
#include "instruments/drums/hats.h"
#include "instruments/drums/kick.h"
#include "instruments/drums/percussion.h"
#include "instruments/drums/snare.h"
#include "patterns/modulators.h"

namespace Vleerhond {
class NordDrumKit : public InstrumentGroup {
   public:
    Kick kick;
    Snare snare;
    Percussion percussion;
    Hats hats;

    NordDrumKit(
        HarmonyStruct& harmony, Modulators& modulators, TimeStruct& time);

    void randomize();
};
}  // namespace Vleerhond