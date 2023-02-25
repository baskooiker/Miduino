#include "snare.h"

#include "instrument_base.h"
#include "patterns/gate_patterns.h"
#include "patterns/modulators.h"
#include "utils/rand.h"

namespace Vleerhond {
Snare::Snare(Modulators& modulators_ref, TimeStruct& time_ref, uint8_t pitch)
    : InstrumentBase(time_ref), pitch(pitch) {}

void Snare::randomize() {
    InstrumentBase::randomize();

    switch (
        Rand::distribution(settings.p_coef, settings.p_off, settings.p_rand)) {
        case 0:  // Coef
            // ofLogVerbose("", "randomize snare coef");
            this->pattern.setCoefSnarePattern();
            break;
        case 1:  // Off
            // ofLogVerbose("", "randomize snare off");
            this->pattern.setAll(0);
            this->pattern.set(4, true);
            this->pattern.set(12, true);
            break;
        case 2:  // Random
            // ofLogNotice("", "randomize snare rand");
            this->pattern.randomize(
                Rand::randf(settings.p_rand_min, settings.p_rand_max));
            switch (
                Rand::distribution(settings.p_length_8, settings.p_length_16)) {
                case 0:
                    pattern.length = 8;
                    break;
                case 1:
                    pattern.length = 16;
                    break;
            }
            break;
    }

    // Timing
    this->timing.randomize();
}

void Snare::play_roll(const TimeStruct& time) {
    static TimeDivision division = TimeDivision::Sixteenth;
    if (Utils::intervalHit(TimeDivision::Sixteenth, time)) {
        uint8_t r = Rand::randui8(16);
        if (r < 3) {
            division = TimeDivision::Thirtysecond;
        } else {
            division = TimeDivision::Sixteenth;
        }
    }

    if (Utils::intervalHit(division, time)) {
        this->midi_channel->noteOn(
            NoteStruct(pitch, getVelocity()), time.getShuffleDelay());
    }
}

bool Snare::play() {
    if (this->snare_roll) {
        this->play_roll(time);
    }

    // Play snare
    if (this->pattern.gate(time) && !this->isKilled()) {
        this->midi_channel->noteOn(
            NoteStruct(pitch, getVelocity()),
            time.getShuffleDelay(this->timing));
        return true;
    }
    return false;
}

uint8_t Snare::getVelocity() {
    // TODO: use modulator
    return 127;
}

void Snare::printPattern() {
    ofLogNotice("", "Snare pattern\n%s", pattern.toString().c_str());
}
}  // namespace Vleerhond