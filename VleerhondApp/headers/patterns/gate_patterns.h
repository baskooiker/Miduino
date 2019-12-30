#pragma once

#include <sstream>
#include <iostream>

#include "ab.h"
#include "core/defs.h"
#include "rand.h"
#include "core/time_struct.h"
#include "patterns/binary_pattern.h"

namespace Vleerhond
{
    class GatePatternAB
    {
    public:
        BinaryPattern patterns[3];
        AbPattern abPattern;
        TimeDivision time_division;
        uint8_t length;

        GatePatternAB();
        bool gate(const TimeStruct& time) const;
        void set_all(bool _value);
        void set(const uint8_t index, bool _value);
        void set_gates_low();
        void randomize(const float prob = .5f);
        void randomize_mask_pattern();
        void set_euclid(const uint8_t length, const uint8_t steps);
        void set_diddles(const float f, const bool starts_with, const uint8_t length);
        void set_coef_pattern(const Coefficients coef);
        void set_coef_kick_pattern();
        void set_coef_snare_pattern();
        void set_coef_hat_pattern();
        void set_coef_slow_pattern();
        void add_one();
        void remove_one();
        std::string toString();
    };
};
