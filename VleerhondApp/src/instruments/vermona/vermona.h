#pragma once

#include "instruments/instrument_rack.h"
#include "instruments/vermona/vermona_fugue.h"
#include "instruments/vermona/vermona_poly.h"

#define VERMONA_UNISONO 0
#define VERMONA_DUOPHONIC 1
#define VERMONA_POLYPHONIC 2

#define CC_VERMONA_MODE 20
#define CC_VERMONA_SYNTH_1_CH 21
#define CC_VERMONA_SYNTH_2_CH 22
#define CC_VERMONA_SYNTH_3_CH 23
#define CC_VERMONA_SYNTH_4_CH 24

namespace Vleerhond {
class Vermona : public InstrumentRack {
   public:
    VermonaPoly poly;
    VermonaFugue fugue;

    Vermona(HarmonyStruct& harmony, TimeStruct& time);
};
}  // namespace Vleerhond
