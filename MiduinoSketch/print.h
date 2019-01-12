#pragma once

#include <stdio.h>

#include "gate.h"

void print_pattern(const BinaryPattern& pattern, const uint8_t length)
{
    for (int i = 0; i < length; i++)
    {
        printf("%3d ", gate(pattern, i));
    }
    printf("\n");
}

void print_pattern(const GatePatternAB& pattern)
{
    for (int i = 0; i < 4; i++)
    {
        print_pattern(pattern.patterns[pattern.abPattern[i]], 16);
    }
}
