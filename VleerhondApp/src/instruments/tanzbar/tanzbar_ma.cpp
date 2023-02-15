#include "instruments/tanzbar/tanzbar_ma.h"

#include "utils/rand.h"

namespace Vleerhond {
TanzbarMa::TanzbarMa(Modulators& modulators_ref, TimeStruct& time_ref)
    : InstrumentBase(time_ref), decay_mod(modulators_ref) {}

void TanzbarMa::randomize() {
    ofLogNotice("tanzbar_ma", "randomize()");
    InstrumentBase::randomize();

    // Randomize Seq
    this->ma_pattern.randomize();
    this->ma_pattern.time_division = TimeDivision::Sixteenth;
    switch (Rand::distribution(32, 32, 32)) {
        case 0:
            this->ma_pattern.length = 2;
            break;
        case 1:
            this->ma_pattern.length = 4;
            break;
        case 2:
            this->ma_pattern.length = 8;
            break;
    }

    // Randomize Interval Pattern
    // IntervalProbs interval_probs(0, Rand::randui8(16), 32);
    interval_pattern.randomizeInterval(IntervalProbs(0, 0, 32));
    interval_pattern.length = pow(Rand::randui8(3, 5), 2);

    // Modulators
    this->decay_mod.randomize(Rand::randui8(128, 64), 0);

    // Timing
    this->timing.randomize();
}
bool TanzbarMa::play() {
    if (isKilled()) return false;

    if (interval_pattern.hit(time)) {
        uint8_t value = 64;
        decay_mod.value(time, value);
        uint8_t decay_value = Utils::quad(value) / 2;

        // TODO: is this good?
        decay_value = Utils::rerange(value, 64, 1);
        getChannel()->sendCC(TB_MA_Decay, decay_value);

        this->midi_channel->noteOn(
            NoteStruct(
                NOTE_TANZBAR_MA,
                Utils::rerange(this->ma_pattern.value(time), 96, 16)),
            time.getShuffleDelay(this->timing));
        return true;
    }
    return false;
}
}  // namespace Vleerhond
