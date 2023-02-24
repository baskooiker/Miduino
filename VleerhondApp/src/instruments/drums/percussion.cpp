#include "percussion.h"

#include "instrument_base.h"
#include "patterns/gate_patterns.h"
#include "patterns/modulators.h"
#include "utils/rand.h"

namespace Vleerhond {
Percussion::Percussion(
    Modulators& modulators_ref, TimeStruct& time_ref, uint8_t pitch)
    : InstrumentBase(time_ref), velocity_mod(modulators_ref), pitch(pitch) {}

void Percussion::randomize() {
    InstrumentBase::randomize();

    switch (Rand::distribution(settings.p_length_8, settings.p_length_16)) {
        case 0:
            pattern.length = 8;
            pattern.abPattern.randomize();
            break;
        case 1:
            pattern.length = 16;
            pattern.abPattern.setConst();
            break;
    }

    switch (Rand::distribution(settings.p_rand, settings.p_drop)) {
        case 0:
            this->pattern.randomize(Rand::randf(.5, 1.0));
            break;
        case 1:
            pattern.setAll(false);
            pattern.addOneGrouped();
            pattern.addOneGrouped();
            break;
    }

    // Modulators
    uint8_t range = Rand::randui8(128, 64);
    this->velocity_mod.randomize(range, 127 - range);

    this->timing.randomize();
}

bool Percussion::play() {
    if (this->isKilled()) {
        return false;
    }

    if (this->pattern.gate(time)) {
        this->midi_channel->noteOn(
            NoteStruct(pitch, getVelocity()),
            time.getShuffleDelay(this->timing));
        return true;
    }

    return false;
}
uint8_t Percussion::getVelocity() {
    uint8_t velocity = 100;
    velocity_mod.value(time, velocity);
    return velocity;
}
}  // namespace Vleerhond