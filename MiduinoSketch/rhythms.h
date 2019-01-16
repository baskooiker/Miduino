#pragma once

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

void set_ab_pattern(uint8_t* ab_pattern)
{
    ab_pattern[0] = 0;
    switch (randi(8))
    {
    case 0: // AAAA
        ab_pattern[1] = 0;
        ab_pattern[2] = 0;
        ab_pattern[3] = 0;
        break;
    case 1: // AAAB
        ab_pattern[1] = 0;
        ab_pattern[2] = 0;
        ab_pattern[3] = 1;
        break;
    case 2: // AABB 
        ab_pattern[1] = 0;
        ab_pattern[2] = 1;
        ab_pattern[3] = 1;
        break;
    case 3: // AABA 
        ab_pattern[1] = 0;
        ab_pattern[2] = 1;
        ab_pattern[3] = 0;
        break;
    case 4: // ABAA 
        ab_pattern[1] = 1;
        ab_pattern[2] = 0;
        ab_pattern[3] = 0;
        break;
    case 5: // ABAB 
        ab_pattern[1] = 1;
        ab_pattern[2] = 0;
        ab_pattern[3] = 1;
        break;
    case 6: // ABAC 
        ab_pattern[1] = 1;
        ab_pattern[2] = 0;
        ab_pattern[3] = 2;
        break;
    case 7: // ABCB
        ab_pattern[1] = 1;
        ab_pattern[2] = 2;
        ab_pattern[3] = 1;
        break;
    default: // AAAA
        ab_pattern[1] = 0;
        ab_pattern[2] = 0;
        ab_pattern[3] = 0;
        break;
    }
}

void set_ab_pattern_const(uint8_t* ab_pattern, const uint8_t value = 0)
{
    for (uint8_t i = 0; i < 4; i++)
    {
        ab_pattern[1] = value;
    }
}

void set_ab_pattern_low(uint8_t* ab_pattern)
{
    ab_pattern[0] = 0;
    switch (randi(4))
    {
    case 0: // AAAB
        ab_pattern[1] = 0;
        ab_pattern[2] = 0;
        ab_pattern[3] = 1;
        break;
    case 1: // AABB 
        ab_pattern[1] = 0;
        ab_pattern[2] = 1;
        ab_pattern[3] = 1;
        break;
    case 2: // AABA 
        ab_pattern[1] = 0;
        ab_pattern[2] = 1;
        ab_pattern[3] = 0;
        break;
    case 3: // ABAA 
        ab_pattern[1] = 1;
        ab_pattern[2] = 0;
        ab_pattern[3] = 0;
        break;
    }
}

void set_ab_pattern_high(uint8_t* ab_pattern)
{
    ab_pattern[0] = 0;
    switch (randi(5))
    {
    case 0: // ABAB 
        ab_pattern[1] = 1;
        ab_pattern[2] = 0;
        ab_pattern[3] = 1;
        break;
    case 1: // ABAC 
        ab_pattern[1] = 1;
        ab_pattern[2] = 0;
        ab_pattern[3] = 2;
        break;
    case 2: // ABCB
        ab_pattern[1] = 1;
        ab_pattern[2] = 2;
        ab_pattern[3] = 1;
        break;
    case 3: // AABC
        ab_pattern[1] = 0;
        ab_pattern[2] = 1;
        ab_pattern[3] = 2;
        break;
    case 4: // ABCA
        ab_pattern[1] = 1;
        ab_pattern[2] = 2;
        ab_pattern[3] = 0;
        break;
    }
}
void randomize(GatePatternAB& pattern, float prob = .5f)
{
    randomize(pattern.patterns[0], prob);
    randomize(pattern.patterns[1], prob);
    randomize(pattern.patterns[2], prob);
    set_ab_pattern(pattern.abPattern);
}
