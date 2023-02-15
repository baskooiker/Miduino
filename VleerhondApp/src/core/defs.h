#pragma once

#include <stdint.h>

#include <map>

#include "core/consts.h"
#include "core/enums.h"

class Coefficients {
   public:
    float one;
    float two;
    float three;
    float four;
    float eights;
    float up;
    float down;
};

class RandomParam {
   public:
    uint8_t note;
    uint8_t min;
    uint8_t max;
};

class CcParam {
   public:
    const uint8_t cc;
    const uint8_t min;
    const uint8_t max;

    CcParam(uint8_t cc, uint8_t min, uint8_t max)
        : cc(cc), min(min), max(max) {}
};

class IntervalProbs {
   public:
    uint8_t p_4;
    uint8_t p_8;
    uint8_t p_16;
    uint8_t p_32;
    uint8_t p_t8;

    IntervalProbs(
        const uint8_t p4 = 0, const uint8_t p8 = 0, const uint8_t p16 = 0,
        const uint8_t p32 = 0, const uint8_t p8t = 0)
        : p_4(p4), p_8(p16), p_16(p16), p_32(p32), p_t8(p8t) {}
};

static const IntervalProbs arp_interval_probs = {25, 25, 25, 0, 0};
