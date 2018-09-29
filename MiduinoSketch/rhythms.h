#ifndef RHYTHMS_H
#define RHYTHMS_H

#include "defs.h"

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

static const uint8_t NR_SD_PATTERNS = 4;
static const uint8_t SD_PATTERNS[NR_SD_PATTERNS][16] = {
  {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
  {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
  {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
};

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

boolean gate(const BinaryPattern& gates, const long step)
{
    return (((gates) & (uint16_t)(0x1 << (uint16_t)(step % 16)))) > 0;
}

boolean gate(const GatePattern16 pattern, const long step)
{
    return gate(pattern.pattern, step % pattern.length);
}

boolean gate(const GatePattern64 pattern, const long step)
{
    uint8_t s = uint8_t(step % pattern.length);
    uint8_t sub_pat = s / 16;
    uint8_t sub_step = s % 16;
    return gate(pattern.patterns[sub_pat], sub_step);
}

boolean gate(const GatePatternAB pattern, const long step)
{
    uint8_t sub_pat = pattern.abPattern[step / 16];
    uint8_t sub_step = step % 16;
    return gate(pattern.patterns[sub_pat], sub_step);
}

void set_gate(BinaryPattern& gates, const uint8_t step, const boolean val)
{
    if (val)
    {
        gates |= ((uint16_t)(0x1) << step);
    }
    else
    {
        gates &= ~((uint16_t)(0x1) << step);
    }
}

GatePattern16 init_pattern(const uint8_t* ar, uint8_t length)
{
    GatePattern16 pat = get_empty_gate_pattern();
    for(uint8_t j = 0; j < length; j++)
    {
        set_gate(pat.pattern, j, ar[j]);
    }
    pat.length = length;
    return pat;
}

void randomize(BinaryPattern& pattern, const float prob)
{
    for (uint8_t j = 0; j < 16; j++)
    {
        set_gate(pattern, j, (random(1024) / 1024.f) < prob);
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

void randomize_ab(GatePattern64* pattern, const float prob)
{
    GatePattern16 pat0 = init_percussive_pattern(prob);
    GatePattern16 pat1 = init_percussive_pattern(prob);
    GatePattern16 pat2 = init_percussive_pattern(prob);
    float pat_prob = (random(1024) / 1024.);
    
    pattern->patterns[0] = pat0.pattern;
    if (pat_prob < .25) // AAAB
    {
        pattern->patterns[1] = pat0.pattern;
        pattern->patterns[2] = pat0.pattern;
        pattern->patterns[3] = pat1.pattern;
    }
    else if (pat_prob < .5) // ABAA
    {
        pattern->patterns[1] = pat1.pattern;
        pattern->patterns[2] = pat0.pattern;
        pattern->patterns[3] = pat0.pattern;
    }
    else if (pat_prob < .625) // AABA
    {
        pattern->patterns[1] = pat0.pattern;
        pattern->patterns[2] = pat1.pattern;
        pattern->patterns[3] = pat0.pattern;
    }
    else if (pat_prob < .75) // ABAC
    {
        pattern->patterns[1] = pat1.pattern;
        pattern->patterns[2] = pat0.pattern;
        pattern->patterns[3] = pat2.pattern;
    }
    else // ABAB
    {
        pattern->patterns[1] = pat1.pattern;
        pattern->patterns[2] = pat0.pattern;
        pattern->patterns[3] = pat1.pattern;
    }
}

GatePattern64 init_percussive_pattern_64(const float prob = .5f)
{
    GatePattern64 pattern = init_gate_pattern_64();
    randomize_ab(&pattern, .5f);
    return pattern;
}

void set_coefficient_pattern(BinaryPattern& pattern,
    float coef_0 = 1.f, // Down-beats
    float coef_1 = 0.f, // Off-beats
    float coef_2 = 0.f, // Up-beats
    float coef_3 = 0.f)
{
    for (int i = 0; i < 16; i += 4)
    {
        set_gate(pattern, i, (random(1024) / 1024.) < coef_0);
    }
    for (int i = 2; i < 16; i += 4)
    {
        set_gate(pattern, i, (random(1024) / 1024.) < coef_1);
    }
    for (int i = 3; i < 16; i += 4)
    {
        set_gate(pattern, i, (random(1024) / 1024.) < coef_2);
    }
    for (int i = 1; i < 16; i += 4)
    {
        set_gate(pattern, i, (random(1024) / 1024.) < coef_3);
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
    }
}

GatePatternAB init_gate_pattern_ab()
{
    GatePatternAB pattern;
    randomize(pattern.patterns[0], .5f);
    set_ab_pattern(pattern.abPattern);
    return pattern;
}

void set_kick_pattern(GatePatternAB& pattern)
{
    set_coefficient_pattern(pattern.patterns[0], 1.f, .25f, .06125f, .06125f);
    set_coefficient_pattern(pattern.patterns[1], 1.f, .25f, .06125f, .06125f);
    set_coefficient_pattern(pattern.patterns[2], 1.f, .25f, .06125f, .06125f);
    set_ab_pattern(pattern.abPattern);
}

#endif // RHYTHM_H
