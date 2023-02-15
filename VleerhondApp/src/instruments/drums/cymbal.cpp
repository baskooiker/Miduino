#include "cymbal.h"

#include "instrument_base.h"
#include "patterns/gate_patterns.h"
#include "patterns/modulators.h"
#include "utils/rand.h"

namespace Vleerhond {
Cymbal::Cymbal(Modulators& modulators_ref, TimeStruct& time_ref)
    : InstrumentBase(time_ref), cy_vel(modulators_ref) {}

void Cymbal::randomize() {
    ofLogNotice("cymbal", "randomize()");
    InstrumentBase::randomize();

    // Randomize Cymbal
    switch (Rand::distribution(0, 16, 16)) {
        case 0:
            // NOT USING HIS ONE!
            this->cy_pattern.setCoefKickPattern();
            this->cy_pattern.length = 16;
            break;
        case 1:
            this->cy_pattern.setEuclid(8, 3);
            this->cy_pattern.length = 8;
            break;
        case 2:
            this->cy_pattern.setCoefHatPattern();
            this->cy_pattern.length = 16;
            break;
    }

    // Modulators
    uint8_t range =
        Rand::randui8(settings.max_velocity - settings.min_velocity);
    this->cy_vel.randomize(range, settings.min_velocity);

    timing.randomize();
}

bool Cymbal::play() {
    // Play Cymbal
    if (this->cy_pattern.gate(time) && !isKilled()) {
        this->midi_channel->noteOn(
            NoteStruct(pitch, getVelocity()),
            time.getShuffleDelay(this->timing));
        return true;
    }
    return false;
}

uint8_t Cymbal::getVelocity() {
    uint8_t velocity = 100;
    this->cy_vel.value(time, velocity);
    return velocity;
}
}  // namespace Vleerhond