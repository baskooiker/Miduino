#pragma once

#include "ab.h"
#include "defs.h"
#include "gate.h"
#include "utils.h"

void randomize(BinaryPattern& pattern, const float prob)
{
    for (uint8_t j = 0; j < 16; j++)
    {
        set_gate(pattern, j, randf() < prob);
    }
}

void randomize(GatePattern16& pattern, const float prob)
{
    randomize(pattern.pattern, prob);
}

void randomize(GatePatternAB& pattern, float prob = .5f)
{
    randomize(pattern.patterns[0], prob);
    randomize(pattern.patterns[1], prob);
    randomize(pattern.patterns[2], prob);
    set_ab_pattern(pattern.abPattern);
}
