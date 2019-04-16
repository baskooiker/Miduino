#pragma once

#include <math.h>
#include "consts.h"

class Rand
{
public:
    static uint8_t randui8(const uint8_t max = 128)
    {
        if (max == 0)
        {
            return 0;
        }
        return rand() % max;
    }
    
    static int8_t randi8(int8_t max_, int8_t min_ = 0)
    {
        int16_t min_v = MIN(max_, min_);
        int16_t max_v = MAX(max_, min_);
        int16_t range = max_v - min_v;
        return (rand() % range) + min_v;
    }
    
    static uint16_t randui16(const uint16_t maximum = 0xFFFF, uint16_t minimum = 0)
    {
        uint16_t _min = MIN(minimum, maximum);
        uint16_t _max = MAX(minimum, maximum);
        uint16_t range = _max - _min;
        if (range == 0)
        {
            return 0;
        }
        return _min + (rand() % range);
    }
    
    static uint8_t randui8(const uint8_t min, const uint8_t max)
    {
        uint8_t _min = MIN(min, max);
        uint8_t _max = MAX(min, max);
        return _min + randui8(_max - _min);
    }
    
    static float randf(const float max = 1.f)
    {
        return ((float)rand() / (float)(RAND_MAX)) * max;
    }
    
    static float randf(const float min, const float max)
    {
        float _min = MIN(min, max);
        float _max = MAX(min, max);
        return _min + randf(_max - _min);
    }
    
    static uint8_t distribution(
        const uint16_t a, 
        const uint16_t b, 
        const uint16_t c = 0, 
        const uint16_t d = 0,
        const uint16_t e = 0,
        const uint16_t f = 0)
    {
        uint16_t total = (uint16_t)a + (uint16_t)b + (uint16_t)c + (uint16_t)d + (uint16_t)e + (uint16_t)f;
        uint16_t r = randui16(total);
        if (r <= a)
            return 0;
        if (r < (a + b))
            return 1;
        if (r < (a + b + c))
            return 2;
        if (r < (a + b + c + d))
            return 3;
        if (r < (a + b + c + d + e))
            return 4;
        return 5;
    }
};