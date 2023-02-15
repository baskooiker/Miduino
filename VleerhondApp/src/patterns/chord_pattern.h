#pragma once

#include "harmony/chord_struct.h"
#include "harmony/chord_type.h"
#include "patterns/cv_patterns.h"

namespace Vleerhond {
class ChordPattern : public CvPattern16 {
   private:
    // TODO: change to vector
    ChordType chord_types[16];

   public:
    ChordPattern();

    void setChordPattern(
        const Scale& scale, const uint8_t start_chord, const bool long_pattern);

    void randomizeChordTypes();

    void initializeChordTypes();

    ChordStruct getChord(const TimeStruct& time) const;
};
}  // namespace Vleerhond
