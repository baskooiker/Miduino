#pragma once

#include "ab.h"
#include "defs.h"
#include "intervals.h"
#include "utils.h"

bool gate(const BinaryPattern& pattern, const uint32_t step, const uint8_t length = 16)
{
    return (pattern >> (step % length)) & 0x1;
}

bool gate(const GatePattern16& pattern, const TimeStruct& time)
{
    if (time.tick % TICKS_PER_STEP != 0) return false;
    return gate(pattern.pattern, (time.tick / TICKS_PER_STEP) % pattern.length);
}

bool gate(const GatePatternAB& pattern, const TimeStruct& time)
{
    if (!interval_hit(pattern.time_division, time)) return false;

    uint8_t pat_length = MIN(pattern.length, 16);
    uint32_t count = get_count(pattern.time_division, time) % (pattern.length <= 16 ? pat_length * 4 : 64);
    return gate(pattern.patterns[pattern.abPattern[count / pat_length]], count % pat_length);
}

void set_gate(BinaryPattern& pattern, const uint8_t index, const bool value)
{
    if (value)
        pattern |= (1 << index);
    else
        pattern &= ~(1 << index);
}

void set_all(GatePatternAB& pattern, bool _value)
{
    uint8_t value = _value ? 0xFF : 0x00;
    for (int i = 0; i < 3; i++)
        pattern.patterns[i] = value;
}

#define NR_OF_TWO_PATTERNS 14
const uint8_t pattern_twos[NR_OF_TWO_PATTERNS][2] = {
    {0,  6},
    {0,  7},
    {0, 10},
    {0, 11},
    {0, 12},
    {0, 12},
    {0, 15},
    {2,  6},
    {2,  7},
    {2, 10},
    {2, 11},
    {2, 12},
    {2, 12},
    {2, 15},
};

void set_gates_low(BinaryPattern& pattern)
{
    pattern = 0x00;
    uint8_t pattern_id = randui8(NR_OF_TWO_PATTERNS);
    for (int i = 0; i < 2; i++)
    {
        set_gate(pattern, pattern_twos[pattern_id][i], true);
    }
}

void set_gates_low(GatePatternAB& pattern)
{
    for (int i = 0; i < 3; i++)
    {
        set_gates_low(pattern.patterns[i]);
    }
    set_ab_pattern(pattern.abPattern);
}
