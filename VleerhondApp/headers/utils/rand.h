#pragma once

#include <stdint.h>

namespace Vleerhond
{
namespace Rand
{
    uint8_t randui8(const uint8_t max = 128);
    int8_t randi8(int8_t max_, int8_t min_ = 0);
    uint16_t randui16(const uint16_t maximum = 0xFFFF, uint16_t minimum = 0);
    uint8_t randui8(const uint8_t min, const uint8_t max);
    float randf(const float max = 1.f);
    float randf(const float min, const float max);
    uint8_t distribution(
        const uint16_t a,
        const uint16_t b,
        const uint16_t c = 0,
        const uint16_t d = 0,
        const uint16_t e = 0,
        const uint16_t f = 0
    );
}
}