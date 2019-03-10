#pragma once

#include "defs.h"

void randomize_mask_pattern(GatePatternAB& pattern)
{
    for (int i = 0; i < 3; i++)
    {
        uint8_t from = randui8(4, 7);
        for (int step = 0; step < 8; step++)
        {
            set_gate(pattern.patterns[i], step, step > from);
        }
    }

    uint8_t r = randui8(3);
    if (r < 1)
    {
        pattern.time_division = TimeDivision::Quarter;
    }
    else if (r < 2)
    {
        pattern.time_division = TimeDivision::Eight;
    }
    else
    {
        pattern.time_division = TimeDivision::Sixteenth;
    }
    pattern.length = 8;
    set_ab_pattern(pattern.abPattern);
}
