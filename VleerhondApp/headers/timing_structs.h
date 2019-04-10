#pragma once

#include "rand.h"

class MicroTimingStruct
{
public:
    int8_t shuffle_off;
    uint8_t delay;

    void randomize()
    {
        this->delay = Rand::randui8();
        this->shuffle_off = Rand::randi8(-20, 20);
    }

};

class TanzbarTimeSettings
{
public:
    MicroTimingStruct bd;
    MicroTimingStruct sd;
    MicroTimingStruct cp;
    MicroTimingStruct hh;
    MicroTimingStruct tc;
    MicroTimingStruct ma;
    MicroTimingStruct cl;
    MicroTimingStruct cb;
    MicroTimingStruct cy;

    void randomize()
    {
        this->bd.randomize();
        this->cb.randomize();
        this->cl.randomize();
        this->cp.randomize();
        this->cy.randomize();
        this->hh.randomize();
        this->ma.randomize();
        this->sd.randomize();
        this->tc.randomize();
    }
};
