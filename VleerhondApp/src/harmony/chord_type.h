#pragma once

#include <vector>

namespace Vleerhond {
enum class ChordType {
    TRIAD,
    SUS2,
    SUS4,
    ADD6,
    SEVEN,
};

ChordType getIrregularChordType();
std::vector<int> indicesForChordType(const ChordType chord_type);
}  // namespace Vleerhond
