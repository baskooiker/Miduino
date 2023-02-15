#include "harmony/chord_type.h"

#include "utils/rand.h"

namespace Vleerhond {
ChordType getIrregularChordType() {
    switch (Rand::distribution(16, 16, 16, 16)) {
        case 0:
            return ChordType::SUS2;
        case 1:
            return ChordType::SUS4;
        case 2:
            return ChordType::ADD6;
        case 3:
            return ChordType::SEVEN;
    }
    return ChordType::TRIAD;
}
std::vector<int> indicesForChordType(const ChordType chord_type) {
    switch (chord_type) {
        default:
        case ChordType::TRIAD:
            return {0, 2, 4};
        case ChordType::SUS2:
            return {0, 1, 4};
        case ChordType::SUS4:
            return {0, 3, 4};
        case ChordType::ADD6:
            return {0, 2, 4, 5};
        case ChordType::SEVEN:
            return {0, 2, 4, 6};
    }
}
}  // namespace Vleerhond
