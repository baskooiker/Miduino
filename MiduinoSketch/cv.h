#pragma once

#include "defs.h"
#include "gate.h"
#include "rhythms.h"

void set_all(CvPatternAB& pattern, const uint32_t value)
{
    for (int i = 0; i < 3; i++)
        pattern.patterns[i].set_all(value);
}

//uint8_t cv(const CvPattern& pattern, const uint32_t step)
//{
//    return pattern[step % 16];
//}

uint8_t cv(const CvPatternAB& pattern, const TimeStruct& time)
{
    uint8_t pat_length = MIN(pattern.length, 16);
    uint32_t count = get_count(pattern.time_division, time) % (pattern.length <= 16 ? pat_length * 4 : 64);
    return pattern.patterns[pattern.abPattern[count / pat_length]].cv(count % pat_length);
}

void randomize(CvPatternAB& pattern, const uint8_t max = 128, const uint8_t min = 0)
{
    for (int i = 0; i < 3; i++)
    {
        pattern.patterns[i].randomize(max, min);
    }
    set_ab_pattern(pattern.abPattern);
}

uint8_t apply_cv(uint8_t input, uint8_t range, int8_t offset)
{
    return (uint8_t)(MIN( MAX(((((int16_t)input) * range) / 128 + offset), 0), 127));
}
