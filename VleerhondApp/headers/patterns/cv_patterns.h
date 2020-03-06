#pragma once

#include <sstream>

#include "core/consts.h"
#include "core/enums.h"
#include "patterns/ab.h"
#include "core/time_struct.h"
#include "utils.h"
#include "harmony/scale.h"
#include "harmony/chords.h"

namespace Vleerhond
{
    class CvPattern
    {
    public:
        uint8_t pattern[STEPS_IN_BAR];

        void randomize(const uint8_t maximum = 128, const uint8_t minimum = 0);
        void set(const uint8_t i, const uint8_t value);
        void set_all(const uint8_t value);
        uint8_t value(const uint8_t step) const;
        std::string toString(const int length = 16);
    };

    class CvPattern16 {
    public:
        CvPattern pattern;
        uint8_t length;
        TimeDivision time_division;

        CvPattern16();
        void randomize(const uint8_t maximum = 128, const uint8_t minimum = 0);
        uint8_t value(const TimeStruct& time) const;
        uint8_t value(const uint8_t step) const;
        void set_all(const uint8_t value);
        void set(const uint8_t index, const uint8_t value);
        // TODO: Move to separate ChordPattern class
        void set_chord_pattern(
            const Scale& scale,
            const uint8_t start_chord,
            const bool long_pattern
        );
    };

    class CvPatternAB
    {
    public:
        CvPattern patterns[3];
        AbPattern abPattern;
        TimeDivision time_division;
        uint8_t length;

        CvPatternAB();
        uint8_t value(const TimeStruct& time) const;
        uint8_t value(const int index) const;
        void set_all(const uint8_t value);
        void randomize(const uint8_t max = 128, const uint8_t min = 0);
        std::string toString();
    };
}
