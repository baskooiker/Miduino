#pragma once

#include "defs.h"
#include "utils.h"

static const uint8_t BD_PATTERNS[][16] = {
  {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0},
  {1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0},
  {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0},
  {1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0},
};
static const uint8_t NR_BD_PATTERNS = sizeof(BD_PATTERNS) / sizeof(*BD_PATTERNS);

static const uint8_t BD_OFF_PATTERNS[][16] = {
  {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0},
  {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0},
};
static const uint8_t NR_BD_OFF_PATTERNS = sizeof(BD_OFF_PATTERNS) / sizeof(*BD_OFF_PATTERNS);

static const uint8_t SD_PATTERNS[][16] = {
  {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
  {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
  {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
  {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
  {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
};
static const uint8_t NR_SD_PATTERNS = sizeof(SD_PATTERNS) / sizeof(*SD_PATTERNS);

static const uint8_t NR_HH_PATTERNS = 4;
static const uint8_t HH_PATTERNS[NR_HH_PATTERNS][16] = {
  {0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 2, 0},
  {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 2},
  {1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1},
  {1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0},
};

GatePattern16 get_empty_gate_pattern()
{
    GatePattern16 gates = {0};
    gates.pattern = 0x00;
    gates.length = 16;
    return gates;
}

GatePattern64 init_gate_pattern_64()
{
    GatePattern64 pattern;
    pattern.length = 64;
    return pattern;
}

BinaryPattern init_binary_pattern(const uint8_t* ar, uint8_t length)
{
    BinaryPattern pattern;
    for (uint8_t j = 0; j < length; j++)
    {
        set_gate(pattern, j, ar[j]);
    }
    return pattern;
}

GatePattern16 init_pattern(const uint8_t* ar, uint8_t length)
{
    GatePattern16 pat = get_empty_gate_pattern();
    pat.pattern = init_binary_pattern(ar, length);
    pat.length = length;
    return pat;
}

void randomize(BinaryPattern& pattern, const float prob)
{
    for (uint8_t j = 0; j < 16; j++)
    {
        set_gate(pattern, j, randomf() < prob);
    }
}

void randomize(GatePattern16& pattern, const float prob)
{
    randomize(pattern.pattern, prob);
}

GatePattern16 init_percussive_pattern(const float prob = .5)
{
    GatePattern16 pat = get_empty_gate_pattern();
    randomize(pat, prob);
    return pat;
}

void randomize_ab(GatePattern64& pattern, const float prob)
{
    GatePattern16 pat0 = init_percussive_pattern(prob);
    GatePattern16 pat1 = init_percussive_pattern(prob);
    GatePattern16 pat2 = init_percussive_pattern(prob);
    float pat_prob = randomf();
    
    pattern.patterns[0] = pat0.pattern;
    if (pat_prob < .25) // AAAB
    {
        pattern.patterns[1] = pat0.pattern;
        pattern.patterns[2] = pat0.pattern;
        pattern.patterns[3] = pat1.pattern;
    }
    else if (pat_prob < .5) // ABAA
    {
        pattern.patterns[1] = pat1.pattern;
        pattern.patterns[2] = pat0.pattern;
        pattern.patterns[3] = pat0.pattern;
    }
    else if (pat_prob < .625) // AABA
    {
        pattern.patterns[1] = pat0.pattern;
        pattern.patterns[2] = pat1.pattern;
        pattern.patterns[3] = pat0.pattern;
    }
    else if (pat_prob < .75) // ABAC
    {
        pattern.patterns[1] = pat1.pattern;
        pattern.patterns[2] = pat0.pattern;
        pattern.patterns[3] = pat2.pattern;
    }
    else // ABAB
    {
        pattern.patterns[1] = pat1.pattern;
        pattern.patterns[2] = pat0.pattern;
        pattern.patterns[3] = pat1.pattern;
    }
}

GatePattern64 init_percussive_pattern_64(const float prob = .5f)
{
    GatePattern64 pattern = init_gate_pattern_64();
    randomize_ab(pattern, .5f);
    return pattern;
}

void set_coefficient_pattern(BinaryPattern& pattern,
    float coef_0 = 1.f, // Down-beats
    float coef_1 = 0.f, // Off-beats
    float coef_2 = 0.f, // Up-beats
    float coef_3 = 0.f)
{
    for (uint8_t i = 0; i < 16; i += 4)
    {
        set_gate(pattern, i, randomf() < coef_0);
    }
    for (uint8_t i = 2; i < 16; i += 4)
    {
        set_gate(pattern, i, randomf() < coef_1);
    }
    for (uint8_t i = 3; i < 16; i += 4)
    {
        set_gate(pattern, i, randomf() < coef_2);
    }
    for (uint8_t i = 1; i < 16; i += 4)
    {
        set_gate(pattern, i, randomf() < coef_3);
    }
}

void set_ab_pattern(uint8_t* ab_pattern)
{
    ab_pattern[0] = 0;
    switch (random(8))
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

GatePatternAB init_gate_pattern_ab()
{
    GatePatternAB pattern;
    pattern.patterns[0] = 0x00;
    pattern.patterns[1] = 0x00;
    pattern.patterns[2] = 0x00;
    randomize(pattern.patterns[0], .5f);
    randomize(pattern.patterns[1], .5f);
    randomize(pattern.patterns[2], .5f);
    set_ab_pattern(pattern.abPattern);
    return pattern;
}

void set_random_pattern_ab(GatePatternAB& pattern, float prob = .5f)
{
    randomize(pattern.patterns[0], prob);
    randomize(pattern.patterns[1], prob);
    randomize(pattern.patterns[2], prob);
    set_ab_pattern(pattern.abPattern);
}

void set_kick_pattern(GatePatternAB& pattern)
{
    set_coefficient_pattern(pattern.patterns[0], 1.f, .25f, .06125f, .06125f);
    set_coefficient_pattern(pattern.patterns[1], 1.f, .25f, .06125f, .06125f);
    set_coefficient_pattern(pattern.patterns[2], 1.f, .25f, .06125f, .06125f);
    set_ab_pattern(pattern.abPattern);
}
