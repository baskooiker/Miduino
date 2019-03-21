#pragma once

#include "defs.h"

class IntervalPattern {
public:
    TimeDivision pattern[16];
    TimeDivision time_division;
    uint8_t length;

    IntervalPattern()
    {
        length = 16;
        for (int i = 0; i < length; i++)
        {
            pattern[i] = TimeDivision::Sixteenth;
        }
        time_division = TimeDivision::Eight;
    }

    TimeDivision interval(const TimeStruct& time) const
    {
        uint32_t c = time.get_count(this->time_division);
        return this->pattern[c % 16];
    }

    bool hit(const TimeStruct& time) const
    {
        return interval_hit(this->interval(time), time);
    }

    void randomize_interval(const IntervalProbs probs)
    {
        for (int i = 0; i < 16; i++)
        {
            if (i % 2 == 0)
            {
                // Alles kan
                switch (distribution(probs.p_4, probs.p_8 * 2, probs.p_16 * 2, probs.p_32 * 2, probs.p_t8 * 2))
                {
                case 0: this->pattern[i] = TimeDivision::Quarter; break;
                case 1: this->pattern[i] = TimeDivision::Eight; break;
                case 2: this->pattern[i] = TimeDivision::Sixteenth; break;
                case 3: this->pattern[i] = TimeDivision::Thirtysecond; break;
                case 4: this->pattern[i] = TimeDivision::TripletEight; break;
                }
            }
            else if (i == 16)
            {
                // Alles behalve triplets en kwarten
                switch (distribution(probs.p_8, probs.p_16, probs.p_32))
                {
                case 0: this->pattern[i] = TimeDivision::Eight; break;
                case 1: this->pattern[i] = TimeDivision::Sixteenth; break;
                case 2: this->pattern[i] = TimeDivision::Thirtysecond; break;
                }
            }
            else
            {
                // alles behalve triplets
                switch (distribution(probs.p_4, probs.p_8 * 2, probs.p_16 * 2, probs.p_32 * 2))
                {
                case 0: this->pattern[i] = TimeDivision::Quarter; break;
                case 1: this->pattern[i] = TimeDivision::Eight; break;
                case 2: this->pattern[i] = TimeDivision::Sixteenth; break;
                case 3: this->pattern[i] = TimeDivision::Thirtysecond; break;
                }
            }

            if (this->pattern[i] == TimeDivision::Quarter
                || this->pattern[i] == TimeDivision::TripletEight)
            {
                this->pattern[i + 1] = this->pattern[i];
                i++;
            }
        }
    }

    void randomize_interval_lead()
    {
        for (int i = 0; i < this->length; i++)
        {
            this->pattern[i] = TimeDivision::Whole;
        }
        for (int bar = 0; bar < 4; bar++)
        {
            // 50% chance for fill in each far
            if (randui8(4) < 3)
            {
                // Starting fill of length 1 or two. Randomize position in bar.
                uint8_t fill_length = randui8(1, 3);
                uint8_t start_beat = randui8(0, 5 - fill_length);
                for (int beat = start_beat; beat < start_beat + fill_length; beat++)
                {
                    TimeDivision time_division = randui8(64) < 8 ?
                        TimeDivision::Sixteenth :
                        TimeDivision::Eight;
                    this->pattern[bar * 4 + beat] = time_division;
                }
            }
        }
        this->time_division = TimeDivision::Quarter;
    }

    void randomize_interval_hat()
    {
        IntervalProbs probs = { 0,  20, 50, 15, 10 };
        probs.p_4 = 0;
        probs.p_8 = randui8(30);
        probs.p_16 = 50;
        probs.p_32 = randui8(64) < 32 ? randui8(15) : 0;
        probs.p_t8 = randui8(64) < 16 ? randui8(15) : 0;
        this->randomize_interval(probs);
    }

};
