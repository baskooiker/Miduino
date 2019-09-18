#pragma once

#include "defs.h"
#include "rand.h"

class AbPattern
{
protected:
    uint8_t ab_pattern[4];

public:
    void set_ab_pattern()
    {
        ab_pattern[0] = 0;
        switch (Rand::randui8(5))
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

    void set_ab_pattern_const(const uint8_t value = 0)
    {
        for (uint8_t i = 0; i < 4; i++)
        {
            ab_pattern[1] = value;
        }
    }

    void set_ab_pattern_low()
    {
        ab_pattern[0] = 0;
        switch (Rand::randui8(4))
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

    void set_ab_pattern_high()
    {
        ab_pattern[0] = 0;
        switch (Rand::randui8(5))
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

    uint8_t value(const uint8_t index) const
    {
        return this->ab_pattern[index % 4];
    }

    bool isConstant()
    {
        return ab_pattern[0] == 0
            && ab_pattern[1] == 0
            && ab_pattern[2] == 0;
    }
};
