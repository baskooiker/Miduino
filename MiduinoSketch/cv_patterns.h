#pragma once

#include "ab.h"
#include "consts.h"
#include "enums.h"
#include "rand.h"
#include "time_struct.h"
#include "utils.h"

class CvPattern
{
public:
    uint8_t pattern[STEPS_IN_BAR];

    void randomize(const uint8_t maximum = 128, const uint8_t minimum = 0)
    {
        for (int i = 0; i < STEPS_IN_BAR; i++)
        {
            this->pattern[i] = randui8(minimum, maximum);
        }
    }

    void set(const uint8_t i, const uint8_t value)
    {
        this->pattern[i] = value;
    }

    void set_all(const uint8_t value)
    {
        for (int i = 0; i < STEPS_IN_BAR; i++)
            this->pattern[i] = value;
    }

    uint8_t value(const uint8_t step) const
    {
        return this->pattern[step % STEPS_IN_BAR];
    }
};

class CvPattern16 {
public:
    CvPattern pattern;
    uint8_t length;
    TimeDivision time_division;

    CvPattern16()
    {
        this->length = STEPS_IN_BAR;
        this->time_division = TimeDivision::Sixteenth;
    }

    void randomize(const uint8_t maximum = 128, const uint8_t minimum = 0)
    {
        this->pattern.randomize(maximum, minimum);
    }

    uint8_t value(const TimeStruct& time) const
    {
        return this->value(get_count(this->time_division, time));
    }

    uint8_t value(const uint8_t step) const
    {
        return this->pattern.value(step % MAX(MIN(this->length, 16), 1));
    }

    void set_all(const uint8_t value)
    {
        this->pattern.set_all(value);
    }

    void set(const uint8_t index, const uint8_t value)
    {
        this->pattern.set(index, value);
    }
};

class CvPatternAB
{
public:
    CvPattern patterns[3];
    uint8_t abPattern[4];
    TimeDivision time_division;
    uint8_t length;

    CvPatternAB()
    {
        length = 64;
        time_division = TimeDivision::Sixteenth;
    }

    uint8_t value(const TimeStruct& time) const
    {
        uint8_t pat_length = MIN(this->length, 16);
        uint32_t count = get_count(this->time_division, time) % (this->length <= 16 ? pat_length * 4 : 64);
        return this->patterns[this->abPattern[count / pat_length]].value(count % pat_length);
    }

    void set_all(const uint8_t value)
    {
        for (int i = 0; i < 3; i++)
            this->patterns[i].set_all(value);
    }

    void randomize(const uint8_t max = 128, const uint8_t min = 0)
    {
        for (int i = 0; i < 3; i++)
        {
            this->patterns[i].randomize(max, min);
        }
        set_ab_pattern(this->abPattern);
    }

};
