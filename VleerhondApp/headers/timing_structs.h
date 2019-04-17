#pragma once

#include "defs.h"
#include "rand.h"

class MicroTimingStruct
{
public:
    int8_t shuffle_off;
    uint8_t delay;

    void randomize()
    {
        this->delay = Rand::randui8();
        this->shuffle_off = Rand::randi8(-10, 10);
    }

};
