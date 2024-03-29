#pragma once

#include <sstream>

#include "core/consts.h"
#include "core/enums.h"
#include "core/time_struct.h"
#include "harmony/chords.h"
#include "harmony/scale.h"
#include "patterns/ab.h"
#include "utils.h"

namespace Vleerhond {
class CvPattern {
   public:
    uint8_t pattern[STEPS_IN_BAR];

    void randomize(const uint8_t maximum = 128, const uint8_t minimum = 0);
    void set(const uint8_t i, const uint8_t value);
    void setAll(const uint8_t value);
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
    void setAll(const uint8_t value);
    void set(const uint8_t index, const uint8_t value);
};

class CvPatternAB {
   public:
    CvPattern patterns[3];
    AbPattern abPattern;
    TimeDivision time_division;
    uint8_t length;

    CvPatternAB();
    uint8_t value(const TimeStruct& time) const;
    uint8_t value(const int index) const;
    void setAll(const uint8_t value);
    void randomize(const uint8_t max = 128, const uint8_t min = 0);
    std::string toString();
};
}  // namespace Vleerhond
