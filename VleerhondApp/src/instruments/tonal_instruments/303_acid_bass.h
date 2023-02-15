#pragma once

#include "bass.h"

namespace Vleerhond {
class TB303Bass : public Bass {
   public:
    TB303Bass(HarmonyStruct& harmony, TimeStruct& time);

    void randomize();
    uint8_t getVelocity();
};
}  // namespace Vleerhond
