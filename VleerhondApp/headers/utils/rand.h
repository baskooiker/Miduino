#pragma once

#include <stdint.h>
#include <vector>

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

    uint8_t distribution(const std::vector<uint16_t>& weights);

    template<typename T>
    T distribute(const std::vector<T>& values, const std::vector<uint16_t>& weights)
    {
        //BOOST_ASSERT_MSG(weights.size() == values.size(), "Size of weights and values is not equal");
        //BOOST_ASSERT_MSG(weights.size() < 7, "Should not use more than 7 weights");
        //BOOST_ASSERT_MSG(weights.size() > 0, "Should use at least 1 weight");

        return values[distribution(weights)];
    }

    template<typename T>
    T distribute(const std::vector<T>& values)
    {
        return distribute<T>(values, std::vector<uint16_t>(values.size(), 16));
    }
}
}
