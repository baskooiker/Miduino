#include "patterns/interval_pattern.h"

#include "utils/rand.h"
#include "utils/utils.h"

namespace Vleerhond {
IntervalPattern::IntervalPattern() {
    length = 16;
    for (size_t i = 0; i < length; i++) {
        pattern[i] = TimeDivision::Sixteenth;
    }
    time_division = TimeDivision::Eighth;
}

TimeDivision IntervalPattern::interval(const TimeStruct& time) const {
    uint32_t c = time.getCount(this->time_division);
    return this->pattern[c % 16];
}

bool IntervalPattern::hit(const TimeStruct& time) const {
    return Utils::intervalHit(this->interval(time), time);
}

void IntervalPattern::randomizeInterval(const IntervalProbs& probs) {
    for (size_t i = 0; i < 16; i++) {
        if (i % 2 == 0) {
            // Alles kan
            switch (Rand::distribution(
                probs.p_4, probs.p_8 * 2, probs.p_16 * 2, probs.p_32 * 2,
                probs.p_t8 * 2)) {
                case 0:
                    this->pattern[i] = TimeDivision::Quarter;
                    break;
                case 1:
                    this->pattern[i] = TimeDivision::Eighth;
                    break;
                case 2:
                    this->pattern[i] = TimeDivision::Sixteenth;
                    break;
                case 3:
                    this->pattern[i] = TimeDivision::Thirtysecond;
                    break;
                case 4:
                    this->pattern[i] = TimeDivision::TripletEight;
                    break;
            }
        } else if (i == 16) {
            // Alles behalve triplets en kwarten
            switch (Rand::distribution(probs.p_8, probs.p_16, probs.p_32)) {
                case 0:
                    this->pattern[i] = TimeDivision::Eighth;
                    break;
                case 1:
                    this->pattern[i] = TimeDivision::Sixteenth;
                    break;
                case 2:
                    this->pattern[i] = TimeDivision::Thirtysecond;
                    break;
            }
        } else {
            // alles behalve triplets
            switch (Rand::distribution(
                probs.p_4, probs.p_8 * 2, probs.p_16 * 2, probs.p_32 * 2)) {
                case 0:
                    this->pattern[i] = TimeDivision::Quarter;
                    break;
                case 1:
                    this->pattern[i] = TimeDivision::Eighth;
                    break;
                case 2:
                    this->pattern[i] = TimeDivision::Sixteenth;
                    break;
                case 3:
                    this->pattern[i] = TimeDivision::Thirtysecond;
                    break;
            }
        }

        if (this->pattern[i] == TimeDivision::Quarter ||
            this->pattern[i] == TimeDivision::TripletEight) {
            this->pattern[i + 1] = this->pattern[i];
            i++;
        }
    }
}

void IntervalPattern::randomizeIntervalLead() {
    for (size_t i = 0; i < this->length; i++) {
        this->pattern[i] = TimeDivision::Whole;
    }
    for (size_t bar = 0; bar < 4; bar++) {
        // 50% chance for fill in each far
        if (Rand::randui8(4) < 3) {
            // Starting fill of length 1 or two. Randomize position in bar.
            uint8_t fill_length = Rand::randui8(1, 3);
            uint8_t start_beat = Rand::randui8(0, 5 - fill_length);
            for (size_t beat = start_beat; beat < start_beat + fill_length;
                 beat++) {
                TimeDivision time_division = Rand::randui8(64) < 8
                                                 ? TimeDivision::Sixteenth
                                                 : TimeDivision::Eighth;
                this->pattern[bar * 4 + beat] = time_division;
            }
        }
    }
    this->time_division = TimeDivision::Quarter;
}

void IntervalPattern::randomizeIntervalHat() {
    IntervalProbs probs = {0, 20, 50, 15, 10};
    probs.p_4 = 0;
    probs.p_8 = Rand::randui8(30);
    probs.p_16 = 50;
    probs.p_32 = Rand::randui8(64) < 32 ? Rand::randui8(15) : 0;
    probs.p_t8 = 0;
    this->randomizeInterval(probs);
}
}  // namespace Vleerhond
