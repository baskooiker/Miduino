#pragma once

#include "defs.h"

void randomize_mask_pattern(GatePatternAB& pattern)
{
    for (int i = 0; i < 3; i++)
    {
        uint8_t from = randi(4, 7);
        for (int step = 0; step < 8; step++)
        {
            set_gate(pattern.patterns[i], step, step > from);
        }
    }

    uint8_t r = randi(3);
    if (r < 1)
    {
        pattern.time_division = TimeDivision::TIME_DIVISION_FOURTH;
    }
    else if (r < 2)
    {
        pattern.time_division = TimeDivision::TIME_DIVISION_EIGHT;
    }
    else
    {
        pattern.time_division = TimeDivision::TIME_DIVISION_SIXTEENTH;
    }
    pattern.length = 8;
    set_ab_pattern(pattern.abPattern);
}
