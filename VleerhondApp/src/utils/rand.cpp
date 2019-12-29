#include "utils/rand.h"

#include <math.h>
#include <cstdlib>

#include "consts.h"

namespace Vleerhond
{
namespace Rand
{
    uint8_t randui8(const uint8_t max)
    {
        if (max == 0)
        {
            return 0;
        }
        return rand() % max;
    }

    int8_t randi8(int8_t max_, int8_t min_)
    {
        int16_t min_v = MIN(max_, min_);
        int16_t max_v = MAX(max_, min_);
        int16_t range = max_v - min_v;
        return (rand() % range) + min_v;
    }

    uint16_t randui16(const uint16_t maximum, uint16_t minimum)
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

    uint8_t randui8(const uint8_t min, const uint8_t max)
    {
        uint8_t _min = MIN(min, max);
        uint8_t _max = MAX(min, max);
        return _min + randui8(_max - _min);
    }

    float randf(const float max)
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
        const uint16_t a,
        const uint16_t b,
        const uint16_t c,
        const uint16_t d,
        const uint16_t e,
        const uint16_t f)
    {
        // Use std::discrete_ditribution
        // https://stackoverflow.com/questions/1761626/weighted-random-numbers
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
}
}