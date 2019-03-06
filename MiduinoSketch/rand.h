#pragma once

#include <math.h>

uint8_t quad(const uint8_t v)
{
    double d = (double)(v / 127.);
    return (uint8_t)(127.9 * d * d);
}

uint8_t log(const uint8_t v)
{
    return (uint8_t)(127.9 * log((double)(v / 127.)));
}

uint8_t randi(const uint8_t max = 128)
{
    if (max == 0)
    {
        return 0;
    }
    return rand() % max;
}

int8_t randi8(int8_t max_, int8_t min_ = 0)
{
    int16_t min_v = MIN(max_, min_);
    int16_t max_v = MAX(max_, min_);
    int16_t range = max_v - min_v;
    return (rand() % range) + min_v;
}

uint16_t randui16(const uint16_t max = 0xFFFF)
{
    if (max == 0)
    {
        return 0;
    }
    return rand() % max;
}

uint8_t randi(const uint8_t min, const uint8_t max)
{
    uint8_t _min = MIN(min, max);
    uint8_t _max = MAX(min, max);
    return _min + randi(_max - _min);
}

float randf(const float max = 1.f)
{
    return ((float)rand() / (float)(RAND_MAX)) * max;
}

float randf(const float min, const float max)
{
    float _min = MIN(min, max);
    float _max = MAX(min, max);
    return _min + randf(_max - _min);
}

uint8_t distribution(
    const uint8_t a, 
    const uint8_t b, 
    const uint8_t c = 0, 
    const uint8_t d = 0,
    const uint8_t e = 0,
    const uint8_t f = 0)
{
    uint16_t total = (uint16_t)a + (uint16_t)b + (uint16_t)c + (uint16_t)d + (uint16_t)e + (uint16_t)f;
    uint16_t r = randui16(total);
    if (r < a)
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

//uint8_t get_distributed_range(uint8_t input, const uint8_t dist, const uint8_t ranges)
//{
//    uint8_t ticks_per_range = 128 / ranges;
//    uint8_t ranges_for_dist = dist / ticks_per_range + 1;
//    ticks_per_range = (uint8_t)((float)ticks_per_range * .7f);
//    if (input > 0)
//    {
//        input = MAX(16, input) - 16;
//    }
//    if (input < dist)
//    {
//        for (int i = 0; i < ranges; i++)
//        {
//            if (input < ((i + 1) * ticks_per_range))
//            {
//                return i + 1;
//            }
//        }
//        return ranges;
//        //return input % (ranges_for_dist + 1);
//    }
//    return 0;
//}
