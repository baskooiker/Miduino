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

boolean gate(BinaryPattern gates, uint8_t step)
{
    return (((gates) & (uint16_t)(0x1 << step))) > 0;
}

//void set_gate(BinaryPattern* gates, uint8_t step)
//{
//    (*gates) |= ((uint16_t)(0x1) << step);
//}

#define BitSet(arg,posn) ((arg) | (1L << (posn)))
#define BitClr(arg,posn) ((arg) & ~(1L << (posn)))
#define BitTst(arg,posn) BOOL((arg) & (1L << (posn)))
#define BitFlp(arg,posn) ((arg) ^ (1L << (posn)))

GatePattern16 init_pattern(uint8_t* ar, uint8_t length)
{
    GatePattern16 pat = get_empty_gate_pattern();
    for(uint8_t j = 0; j < length; j++)
    {
        if(ar[j] > 0)
        {
            pat.pattern |= (uint16_t)(0x0001 << j);
        }
    }
//   pat.pattern |= (uint16_t)(0x0001 << 0);
//   pat.pattern |= (uint16_t)(0x0001 << 2);
//   pat.pattern |= (uint16_t)(0x0100);
   return pat;
}


