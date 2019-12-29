#pragma once

#include <sstream>
#include <iostream>

#include "ab.h"
#include "defs.h"
#include "rand.h"
#include "time_struct.h"

namespace Vleerhond
{
    class BinaryPattern
    {
    public:
        uint16_t pattern;

        void randomize(const float prob);
        void set_gate(const uint8_t index, const bool value);
        bool gate(const uint32_t step, const uint8_t length = 16) const;
        void set_gates_low();
        void set_kick_fill(uint8_t offset);
        void set_euclid(const uint8_t _length, const uint8_t _steps);
        std::vector<uint8_t> distribute(uint8_t size, uint8_t amount);
        void set_diddles(const float f, const bool starts_with, uint8_t length);
        void set_coef_pattern(const Coefficients coef);
        void add_one(const uint8_t length);
        void remove_one(const uint8_t length);
        void shift_one();
        bool shift_down();
        bool shift_up();
        std::string toString();
    };

};