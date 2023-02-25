#pragma once

#include "moog_bass.h"
#include "patterns/modulators.h"

namespace Vleerhond {
class MoogLongBass : public MoogBass {
   public:
    MoogLongBass(
        HarmonyStruct& harmony, Modulators& modulators, TimeStruct& time);
    virtual void randomize();
};
}  // namespace Vleerhond
