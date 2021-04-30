#pragma once

#include "core/defs.h"

namespace Vleerhond
{
class AbPattern
{
protected:
    uint8_t ab_pattern[4];

public:
    void randomize();

    void setConst(const uint8_t value = 0);

    void setLow();

    void setHigh();
    
    void setABCB();
    
    uint8_t value(const uint8_t index) const;

    bool isConstant();
};
}