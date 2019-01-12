#pragma once

#include "defs.h"
#include "gate.h"

void set_all(CvPattern& pattern, const uint32_t value)
{
    for (int i = 0; i < 16; i++)
        pattern[i] = value;
}

void set_all(CvPatternAB& pattern, const uint32_t value)
{
    for (int i = 0; i < 3; i++)
        set_all(pattern.patterns[i], value);
}

uint8_t cv(const CvPattern& pattern, const uint32_t step)
{
    // TODO
    return pattern[step % 16];
}

uint8_t cv(const CvPatternAB& pattern, const uint32_t step)
{
    // TODO
    uint32_t count = get_count(pattern.time_division, step, 0);
    uint8_t pat_length = MIN(pattern.length, 16);
    uint8_t sub_step = count % pat_length;
    uint8_t sub_part = count / pat_length;
    return cv(pattern.patterns[sub_part], sub_step);
}

void randomize(CvPatternAB& pattern, const uint8_t max = 128, const uint8_t min = 0)
{
    // TODO
}
