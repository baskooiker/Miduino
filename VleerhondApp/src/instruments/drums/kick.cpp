#include "kick.h"

#include "instrument_base.h"
#include "patterns/gate_patterns.h"
#include "patterns/modulators.h"
#include "utils/rand.h"

namespace Vleerhond {
Kick::Kick(Modulators& modulators_ref, TimeStruct& time_ref, uint8_t pitch)
    : InstrumentBase(time_ref), pitch(pitch) {}

void Kick::randomize() {
    // ofLogVerbose("kick", "randomize");
    InstrumentBase::randomize();

    bd_pattern.setAll(false);
    ghost_notes.setAll(false);
    bd_pattern.abPattern.setConst();

    for (int i : {0, 4, 8, 12}) {
        bd_pattern.set(i, true);
    }

    switch (Rand::distribution(16, 16, 0)) {
        case 0: {
            ofLogVerbose("kick", "randomize_kick: default");
            this->randomize_kick();
            break;
        }
        case 1: {
            ofLogVerbose("kick", "randomize_kick: coeff");
            Coefficients ghost_coef = {0};
            ghost_coef.eights = Rand::randf(.25);
            ghost_coef.up = Rand::randf(.25);
            ghost_coef.down = Rand::randf(.25);
            this->ghost_notes.setCoefPattern(ghost_coef);
            break;
        }
        default:
            ofLogVerbose("kick", "randomize_kick: NOT!");
            break;
    }

    this->timing.randomize();
}

bool Kick::play() {
    if (this->isKilled()) {
        return false;
    }

    if (this->bd_pattern.gate(time)) {
        this->midi_channel->noteOn(
            NoteStruct(this->pitch, getVelocity()),
            time.getShuffleDelay(this->timing));
        return true;
    } else if (ghost_notes.gate(time) && settings.play_ghost_notes) {
        this->midi_channel->noteOn(
            NoteStruct(this->pitch, getVelocity()),
            time.getShuffleDelay(this->timing));
        return true;
    }
    return false;
}

uint8_t Kick::getVelocity() {
    return this->bd_pattern.gate(time) ? settings.max_velocity
                                       : settings.min_velocity;
}

void Kick::randomize_kick() {
    // Fill in first or second half of bar
    uint8_t half = Rand::distribution(64, 64);

    this->bd_pattern.patterns[1].setKickFill(half * 8);
    this->bd_pattern.abPattern.randomize();
}

}  // namespace Vleerhond
