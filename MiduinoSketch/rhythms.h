#include "defs.h"

typedef uint16_t BinaryPattern;

typedef struct {
    BinaryPattern pattern;
    uint8_t length;
} GatePattern16;

GatePattern16 get_empty_gate_pattern()
{
    GatePattern16 gates = {0};
    gates.pattern = 0x00;
    gates.length = 16;
    return gates;
}

typedef struct {
    BinaryPattern patterns[4];
    uint8_t length;
} GatePattern64;

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

#define BitSet(arg,posn) ((arg) | (1L << (posn)))
#define BitClr(arg,posn) ((arg) & ~(1L << (posn)))
#define BitTst(arg,posn) BOOL((arg) & (1L << (posn)))
#define BitFlp(arg,posn) ((arg) ^ (1L << (posn)))

GatePattern16 init_pattern(uint8_t* ar, uint8_t length)
{
    GatePattern16 pat = get_empty_gate_pattern();
    for(uint8_t j = 0; j < length; j++)
    {
        set_gate(&pat.pattern, j, ar[j]);
    }
   return pat;
}

GatePattern16 init_percussive_pattern()
{
    GatePattern16 pat = get_empty_gate_pattern();
    for(uint8_t j = 0; j < pat.length; j++)
    {
        set_gate(&pat.pattern, j, random(128) < 64);
    }
    return pat;
}


GatePattern64 init_percussive_pattern_64()
{
    GatePattern64 pattern = init_gate_pattern_64();
    GatePattern16 pat0 = init_percussive_pattern();
    GatePattern16 pat1 = init_percussive_pattern();
    GatePattern16 pat2 = init_percussive_pattern();
    uint8_t prob = random(265);
    
    pattern.patterns[0] = pat0.pattern;
    if (prob < 128) // AAAB
    {
        pattern.patterns[1] = pat0.pattern;
        pattern.patterns[2] = pat0.pattern;
        pattern.patterns[3] = pat1.pattern;
    }
    else if (prob < 192) // ABAC
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
    return pattern;
}

