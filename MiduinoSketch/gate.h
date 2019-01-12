#pragma once

bool gate(const BinaryPattern& pattern, const uint32_t step)
{
    // TODO
    return (pattern >> (step % 16)) & 0x1;
}

bool gate(const GatePattern16& pattern, const uint32_t step, const uint8_t tick)
{
    // TODO
    if (tick % TICKS_PER_STEP != 0) return false;
    return gate(pattern.pattern, step % pattern.length);
}

uint32_t get_count(const TimeDivision time_division, const uint32_t step, const uint8_t tick)
{
    return (step * TICKS_PER_STEP + tick) / (uint8_t)time_division;
}

bool gate(const GatePatternAB& pattern, const uint32_t step, const uint8_t tick)
{
    // TODO
    uint32_t count = get_count(pattern.time_division, step, tick);
    uint8_t pat_length = MIN(pattern.length, 16);
    uint8_t sub_step = count % pat_length;
    uint8_t sub_part = count / pat_length;
    return gate(pattern.patterns[sub_part], sub_step);
}

void set_gate(BinaryPattern& pattern, const uint8_t index, const bool value)
{
    // TODO
    if (value)
        pattern |= (1 << index);
    else
        pattern &= ~(1 << index);
}
