#pragma once

#include <iostream>
#include <sstream>

#include "ab.h"
#include "core/defs.h"
#include "core/time_struct.h"
#include "patterns/binary_pattern.h"

namespace Vleerhond {
class GatePatternAB {
   public:
    BinaryPattern patterns[3];
    AbPattern abPattern;
    TimeDivision time_division;
    uint8_t length;

    GatePatternAB();
    bool gate(const TimeStruct& time) const;
    void setAll(bool _value);
    void set(const uint8_t index, bool _value);
    void setGatesLow();
    void randomize(const float prob = .5f);
    void randomize_mask_pattern();
    void setEuclid(const uint8_t length, const uint8_t steps);
    void setDiddles(
        const float f, const bool starts_with, const uint8_t length);
    void setCoefPattern(const Coefficients coef);
    void setCoefKickPattern();
    void setCoefSnarePattern();
    void setCoefHatPattern();
    void setCoefSlowPattern();
    void addOneGrouped();
    void removeOne();
    std::string toString();
};
};  // namespace Vleerhond
