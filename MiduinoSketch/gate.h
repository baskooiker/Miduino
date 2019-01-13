#pragma once

#include "defs.h"
#include "intervals.h"

bool gate(const BinaryPattern& pattern, const uint32_t step)
{
    return (pattern >> (step % 16)) & 0x1;
}

bool gate(const GatePattern16& pattern, const uint32_t step, const uint8_t tick)
{
    if (tick % TICKS_PER_STEP != 0) return false;
    return gate(pattern.pattern, step % pattern.length);
}

bool gate(const GatePatternAB& pattern, const uint32_t step, const uint8_t tick)
{
    if (!interval_hit(pattern.time_division, step, tick)) return false;

    uint8_t pat_length = MIN(pattern.length, 16);
    uint8_t total_length = 64;
    if (pattern.length <= 16)
    {
        total_length = pat_length * 4;
    }
    uint32_t count = get_count(pattern.time_division, step, tick) % total_length;
    uint8_t sub_step = count % pat_length;
    uint8_t sub_part = count / pat_length;
    return gate(pattern.patterns[pattern.abPattern[sub_part]], sub_step);
}

void set_gate(BinaryPattern& pattern, const uint8_t index, const bool value = true)
{
    if (value)
        pattern |= (1 << index);
    else
        pattern &= ~(1 << index);
}
