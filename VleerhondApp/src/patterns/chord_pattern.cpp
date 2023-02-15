#include "patterns/chord_pattern.h"

#include "utils/rand.h"

namespace Vleerhond {
ChordPattern::ChordPattern() { initializeChordTypes(); }

void ChordPattern::setChordPattern(
    const Scale& scale, const uint8_t start_chord, const bool long_pattern) {
    std::vector<uint8_t> options = scale.getAvailableChordsIndices();
    std::random_shuffle(options.begin(), options.end());

    uint8_t start_chord_idx = 0;
    for (size_t i = 0; i < options.size(); i++) {
        if (start_chord == options[i]) {
            start_chord_idx = i;
            break;
        }
    }
    std::iter_swap(options.begin(), options.begin() + start_chord_idx);

    std::vector<uint8_t> chord_time_pattern =
        long_pattern ? ChordUtils::getChordTimePatternLong()
                     : ChordUtils::getChordTimePatternShort();

    std::vector<uint8_t> seq =
        ChordUtils::getChordSeq(options, chord_time_pattern.size());

    uint8_t c = 0;
    for (size_t i = 0; i < chord_time_pattern.size(); i++) {
        for (size_t j = 0; j < chord_time_pattern[i]; j++) {
            this->set(c, seq[i]);
            c += 1;
        }
    }
    this->length = c;

    randomizeChordTypes();
}

void ChordPattern::randomizeChordTypes() {
    initializeChordTypes();
    for (size_t i = 0; i < 16; i++) {
        if (Rand::distribution(16, 16) == 1) {
            ChordType chord_type = getIrregularChordType();
            chord_types[i++] = chord_type;
            chord_types[i] = chord_type;
        } else {
            i++;
        }
    }
}

void ChordPattern::initializeChordTypes() {
    for (size_t i = 0; i < 16; i++) {
        chord_types[i] = ChordType::TRIAD;
    }
}

ChordStruct ChordPattern::getChord(const TimeStruct& time) const {
    // TODO: Vary time division?
    return ChordStruct(
        this->value(time),
        chord_types[time.getCount(TimeDivision::Quarter) % 16]);
}

}  // namespace Vleerhond
