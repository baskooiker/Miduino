#pragma once

#include "harmony/harmony_struct.h"
#include "mono.h"
#include "patterns/modulators.h"

namespace Vleerhond {
class VermonaMono : public Mono {
   public:
    VermonaMono(
        HarmonyStruct& harmony, Modulators& modulators, TimeStruct& time)
        : Mono(harmony, modulators, time, MIDI_CHANNEL_MONO) {}

    bool play() {
        if (this->getHit()) {
            getChannel()->sendCC(MODULATION_WHEEL_CC, getVelocity());
        }
        return Mono::play();
    }
};
}  // namespace Vleerhond
