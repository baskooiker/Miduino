#pragma once

#include "defs.h"
#include "gate.h"
#include "rhythms.h"

void set_euclid(BinaryPattern& pattern, const uint8_t _length, const uint8_t _steps)
{
    uint8_t length = MIN(_length, 16);
    uint8_t steps = MIN(_steps, length);

    pattern = 0x00;

    uint8_t counters[16] = { 0 };
    for (int i = 0; i < length; i++)
        counters[i%steps]++;

    randomize_order(counters, steps);

    uint8_t c = 0;
    for (int i = 0; i < steps; i++)
    {
        set_gate(pattern, c);
        c += counters[i];
    }
}

void set_euclid(GatePatternAB& pattern, const uint8_t length, const uint8_t steps)
{
    for (int i = 0; i < 3; i++)
        set_euclid(pattern.patterns[i], length, steps);
    set_ab_pattern(pattern.abPattern);
}
