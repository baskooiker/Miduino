#pragma once

#include "defs.h"

TimeDivision interval(const IntervalPattern& pattern, const uint32_t step, const uint8_t tick)
{
    // TODO
    uint32_t c = get_count(pattern.time_division, step, tick);
    return pattern.pattern[c % 16];
}

bool interval_hit(const TimeDivision time_division, const uint32_t step, const uint8_t tick)
{
    // TODO
    return (step * TICKS_PER_STEP + tick) % (uint8_t)time_division == 0;
}

bool interval_hit(const IntervalPattern& pattern, const uint32_t step, const uint8_t tick)
{
    // TODO
    return interval_hit(interval(pattern, step, tick), step, tick);
}

void randomize_interval(IntervalPattern& pattern, const IntervalProbs probs)
{
    // TODO
}

