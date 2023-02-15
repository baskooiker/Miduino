#pragma once

#include "harmony/harmony_struct.h"
#include "mono_dub.h"
#include "patterns/modulators.h"

namespace Vleerhond {
class VermonaMonoDub : public MonoDub {
   private:
    // TODO: use this
    ModulationReceiver velocity_mod;

   public:
    VermonaMonoDub(
        Mono& mono, HarmonyStruct& harmony, TimeStruct& time,
        Modulators& modulators)
        : MonoDub(mono, harmony, time, MIDI_CHANNEL_MONO_2),
          velocity_mod(modulators) {}

    void randomize() {
        velocity_mod.randomize(Rand::randui8(128), 0, .5);
        MonoDub::randomize();
    }

    uint8_t getVelocity() {
        uint8_t value = 0;
        velocity_mod.value(time, value);
        return value;
    }

    bool play() {
        if (this->lead.getHit()) {
            getChannel()->sendCC(MODULATION_WHEEL_CC, getVelocity());
        }
        return MonoDub::play();
    }
};
}  // namespace Vleerhond
