#include "toms.h"

#include "instrument_base.h"
#include "patterns/gate_patterns.h"
#include "patterns/interval_pattern.h"
#include "patterns/modulators.h"
#include "utils/rand.h"

namespace Vleerhond {
Toms::Toms(Modulators& modulators_ref, TimeStruct& time_ref)
    : InstrumentBase(time_ref), tom_vel(modulators_ref) {}

void Toms::randomize() {
    ofLogNotice("toms", "randomize()");
    InstrumentBase::randomize();

    switch (Rand::distribution(0, 16)) {
        case 0: {
            // Randomize toms
            this->tom_pattern.randomize(1, 127);
            for (size_t i = 0; i < 3; i++) {
                for (size_t j = 0; j < 16; j++) {
                    if (Rand::randf() < .2) {
                        this->tom_pattern.patterns[j].set(i, false);
                    }
                }
            }
        } break;
        case 1: {
            tom_pattern.setAll(0);
            int16_t length = 8;
            if (pitches.size() < 4) {
                length = Rand::randf() < .5 ? 8 : 16;
            }
            tom_pattern.abPattern.setConst();
            for (size_t i = 0; i < pitches.size(); i++) {
                std::vector<uint8_t> opts;
                for (int16_t j = 0; j < length; j++) {
                    opts.push_back(j);
                }
                std::random_shuffle(opts.begin(), opts.end());
                for (uint8_t opt : opts) {
                    if (tom_pattern.patterns[0].value(opt) == 0 &&
                        tom_pattern.patterns[0].value(opt + 1 % length) == 0) {
                        tom_pattern.patterns[0].set(opt, i + 1);
                        tom_pattern.patterns[0].set(opt + 1 % length, i + 1);
                        break;
                    }
                }
            }
        } break;
    }

    // Modulators
    uint8_t range = Rand::randui8(16);
    this->tom_vel.randomize(range, 127 - range);

    this->timing.randomize();
}

bool Toms::play() {
    if (isKilled()) {
        return false;
    }

    // Play toms
    uint8_t tom_prob = this->tom_pattern.value(time);
    if (Utils::intervalHit(TimeDivision::Sixteenth, time) && tom_prob > 0) {
        this->midi_channel->noteOn(
            NoteStruct(getPitch(time), getVelocity()),
            time.getShuffleDelay(this->timing));
        return true;
    }
    return false;
}

uint8_t Toms::getVelocity() {
    uint8_t velocity = 100;
    this->tom_vel.value(time, velocity);
    return velocity;
}

uint8_t Toms::getPitch(const TimeStruct& time) {
    uint8_t tom_prob = this->tom_pattern.value(time);
    return pitches.at(tom_prob % pitches.size());
}
}  // namespace Vleerhond