#pragma once

#include <sstream>
#include <iostream>

#include "ab.h"
#include "core/defs.h"
#include "core/time_struct.h"

namespace Vleerhond
{
    class BinaryPattern
    {
    public:
        uint16_t pattern;

        void randomize(const float prob);
        void setGate(const uint8_t index, const bool value);
        bool gate(const uint32_t step, const uint8_t length = 16) const;
        void setGatesLow();
        void setKickFill(uint8_t offset);
        void setEuclid(const uint8_t _length, const uint8_t _steps);
        std::vector<uint8_t> distribute(uint8_t size, uint8_t amount);
        void setDiddles(const float f, const bool starts_with, uint8_t length);
        void setCoefPattern(const Coefficients coef);
        void addOne(const uint8_t length);
        void removeOne(const uint8_t length);
        void shiftOne();
        bool shiftDown();
        bool shiftUp();
        std::string toString();
    };

};