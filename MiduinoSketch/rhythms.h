
#ifndef RHYTHMS_H
#define RHYTHMS_H

#include "defs.h"

static const uint8_t NR_BD_PATTERNS = 4;
static const uint8_t BD_PATTERNS[NR_BD_PATTERNS][16] = {
  {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0},
  {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0},
  {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0},
  {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0},
};

static const uint8_t NR_SD_PATTERNS = 4;
static const uint8_t SD_PATTERNS[NR_SD_PATTERNS][16] = {
  {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
  {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
  {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
};

static const uint8_t NR_HH_PATTERNS = 4;
static const uint8_t HH_PATTERNS[NR_SD_PATTERNS][16] = {
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

boolean gate(const BinaryPattern gates, const long step)
{
    return (((gates) & (uint16_t)(0x1 << (uint16_t)(step % 16)))) > 0;
}

boolean gate(const GatePattern16 pattern, const long step)
{
    return gate(pattern.pattern, step);
}

boolean gate(const GatePattern64 pattern, const long step)
{
    uint8_t s = uint8_t(step % pattern.length);
    uint8_t sub_pat = s / 16;
    uint8_t sub_step = s % 16;
    return gate(pattern.patterns[sub_pat], sub_step);
}

void set_gate(BinaryPattern* gates, const uint8_t step, const boolean val)
{
    if (val)
    {
        (*gates) |= ((uint16_t)(0x1) << step);
    }
    else
    {
        (*gates) &= ~((uint16_t)(0x1) << step);
    }
}

GatePattern16 init_pattern(const uint8_t* ar, uint8_t length)
{
    GatePattern16 pat = get_empty_gate_pattern();
    for(uint8_t j = 0; j < length; j++)
    {
        set_gate(&pat.pattern, j, ar[j]);
    }
   return pat;
}

void randomize(GatePattern16* pattern, const float prob)
{
    for(uint8_t j = 0; j < pattern->length; j++)
    {
        set_gate(&pattern->pattern, j, (random(1024) / 1024.f) < prob);
    }
}

GatePattern16 init_percussive_pattern()
{
    GatePattern16 pat = get_empty_gate_pattern();
    randomize(&pat, .5f);
    return pat;
}

void randomize_ab(GatePattern64* pattern, const float prob)
{
    GatePattern16 pat0 = init_percussive_pattern();
    GatePattern16 pat1 = init_percussive_pattern();
    GatePattern16 pat2 = init_percussive_pattern();
    uint8_t pat_prob = random(1024);
    
    pattern->patterns[0] = pat0.pattern;
    if (pat_prob < 128) // AAAB
    {
        pattern->patterns[1] = pat0.pattern;
        pattern->patterns[2] = pat0.pattern;
        pattern->patterns[3] = pat1.pattern;
    }
    else if (pat_prob < 192) // ABAC
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

#endif // RHYTHM_H
