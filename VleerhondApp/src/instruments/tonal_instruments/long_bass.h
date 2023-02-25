#pragma once

#include "bass.h"
#include "patterns/modulators.h"

namespace Vleerhond {
class LongBass : public Bass {
   public:
    LongBass(HarmonyStruct& harmony, Modulators& modulators, TimeStruct& time);
    virtual void randomize();
};
}  // namespace Vleerhond
