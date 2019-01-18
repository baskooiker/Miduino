#pragma once

#include <stdio.h>

#include "gate.h"

void print_pattern(const BinaryPattern& pattern, const uint8_t length)
{
    for (int i = 0; i < length; i++)
    {
        printf("%-3d ", gate(pattern, i));
    }
    printf("\n");
}

void print_pattern(const GatePatternAB& pattern)
{
    for (int i = 0; i < 4; i++)
    {
        print_pattern(pattern.patterns[pattern.abPattern[i]], 16);
    }
    printf("\n");
}

void print_pattern(const CvPattern& pattern)
{
    for (int i = 0; i < 16; i++)
    {
        printf("%-3d ", cv(pattern, i));
    }
    printf("\n");
}

void print_pattern(const CvPatternAB& pattern)
{
    for (int i = 0; i < 4; i++)
    {
        print_pattern(pattern.patterns[pattern.abPattern[i]]);
    }
    printf("\n");
}

void print_storage(PitchStorage& storage)
{
    printf("\nsize: %d\n", storage.size);
    for (int i = 0; i < storage.size; i++)
    {
        printf("%2d %3d\n", storage.data[i].pitch, storage.data[i].length);
    }
}
