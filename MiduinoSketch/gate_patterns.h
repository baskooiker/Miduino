#pragma once

#include "ab.h"
#include "defs.h"
#include "rand.h"
#include "time_struct.h"
#include "intervals.h"

#define NR_OF_TWO_PATTERNS 14
const uint8_t pattern_twos[NR_OF_TWO_PATTERNS][2] = {
    {0,  6},
    {0,  7},
    {0, 10},
    {0, 11},
    {0, 12},
    {0, 12},
    {0, 15},
    {2,  6},
    {2,  7},
    {2, 10},
    {2, 11},
    {2, 12},
    {2, 12},
    {2, 15},
};

class BinaryPattern
{
public:
    uint16_t pattern;

    void randomize(const float prob)
    {
        for (uint8_t j = 0; j < 16; j++)
        {
            set_gate(j, randf() < prob);
        }
    }

    void set_gate(const uint8_t index, const bool value)
    {
        if (value)
            this->pattern |= (1 << index);
        else
            this->pattern &= ~(1 << index);
    }

    bool gate(const uint32_t step, const uint8_t length = 16) const
    {
        return (this->pattern >> (step % length)) & 0x1;
    }

    void set_gates_low()
    {
        this->pattern = 0x00;
        uint8_t pattern_id = randui8(NR_OF_TWO_PATTERNS);
        for (int i = 0; i < 2; i++)
        {
            this->set_gate(pattern_twos[pattern_id][i], true);
        }
    }

};

class GatePattern16
{
public:
    BinaryPattern pattern;
    uint8_t length;

    GatePattern16()
    {
        pattern.pattern = 0x00;
        length = 16;
    }

    bool gate(const TimeStruct& time) const
    {
        if (time.tick % TICKS_PER_STEP != 0)
            return false;
        return this->pattern.gate((time.tick / TICKS_PER_STEP) % this->length);
    }

    void randomize(const float prob)
    {
        this->pattern.randomize(prob);
    }

};

class GatePatternAB
{
public:
    BinaryPattern patterns[3];
    uint8_t abPattern[4];
    TimeDivision time_division;
    uint8_t length;

    GatePatternAB()
    {
        patterns[0].pattern = 0x00;
        patterns[1].pattern = 0x00;
        patterns[2].pattern = 0x00;
        length = 64;
        time_division = TimeDivision::Sixteenth;
    }

    bool gate(const TimeStruct& time) const
    {
        if (!interval_hit(this->time_division, time)) return false;

        uint8_t pat_length = MIN(this->length, 16);
        uint32_t count = get_count(this->time_division, time) % (this->length <= 16 ? pat_length * 4 : 64);
        return this->patterns[this->abPattern[count / pat_length]].gate(count % pat_length);
    }

    void set_all(bool _value)
    {
        uint8_t value = _value ? 0xFF : 0x00;
        for (int i = 0; i < 3; i++)
            this->patterns[i].pattern = value;
    }

    void set_gates_low()
    {
        for (int i = 0; i < 3; i++)
        {
            this->patterns[i].set_gates_low();
        }
        set_ab_pattern(this->abPattern);
    }

    void randomize(float prob = .5f)
    {
        for (int i = 0; i < 3; i++)
            this->patterns[i].randomize(prob);
        set_ab_pattern(this->abPattern);
    }

};
