#pragma once

#include "core/defs.h"
#include "core/time_struct.h"

namespace Vleerhond {
class IntervalPattern {
   public:
    TimeDivision pattern[16];
    TimeDivision time_division;
    uint8_t length;

    IntervalPattern();

    TimeDivision interval(const TimeStruct& time) const;

    bool hit(const TimeStruct& time) const;

    void randomizeInterval(const IntervalProbs& probs);

    void randomizeIntervalLead();

    void randomizeIntervalHat();
};
}  // namespace Vleerhond