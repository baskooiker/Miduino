#pragma once

#include "core/defs.h"
#include "rand.h"

namespace Vleerhond
{
class AbPattern
{
protected:
    uint8_t ab_pattern[4];

public:
    void set_ab_pattern();

    void set_ab_pattern_const(const uint8_t value = 0);

    void set_ab_pattern_low();

    void set_ab_pattern_high();

    uint8_t value(const uint8_t index) const;

    bool isConstant();
};
}