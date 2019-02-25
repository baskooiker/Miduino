#pragma once

#include "defs.h"
#include "gate.h"
#include "rhythms.h"

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
    return pattern[step % 16];
}

uint8_t cv(const CvPattern16& pattern, const uint32_t step)
{
    return pattern.pattern[step % MAX(MIN(pattern.length, 16), 1)];
}

uint8_t cv(const CvPatternAB& pattern, const uint32_t step)
{
    uint8_t pat_length = MIN(pattern.length, 16);
    uint32_t count = get_count(pattern.time_division, step, 0) % (pattern.length <= 16 ? pat_length * 4 : 64);
    return cv(pattern.patterns[pattern.abPattern[count / pat_length]], count % pat_length);
}

void randomize(CvPattern& pattern, const uint8_t max = 128, const uint8_t min = 0)
{
    for (int i = 0; i < 16; i++)
    {
        pattern[i] = randi(min, max);
    }
}

void randomize(CvPatternAB& pattern, const uint8_t max = 128, const uint8_t min = 0)
{
    for (int i = 0; i < 3; i++)
    {
        randomize(pattern.patterns[i], max, min);
    }
    set_ab_pattern(pattern.abPattern);
}

uint8_t apply_cv(uint8_t input, uint8_t range, int8_t offset)
{
    return (uint8_t)(MIN( MAX(((((int16_t)input) * range) / 128 + offset), 0), 127));
}
