#pragma once

#include "bass.h"

namespace Vleerhond {
class AcidBass : public Bass {
   private:
    uint8_t accent_amount;

   public:
    AcidBass(HarmonyStruct& harmony, TimeStruct& time);
    virtual void randomize();
    virtual bool play();
};
}  // namespace Vleerhond
