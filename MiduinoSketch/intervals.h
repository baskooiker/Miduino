#pragma once

#include "defs.h"
#include "rand.h"

uint32_t get_count(const TimeDivision time_division, const uint32_t step, const uint8_t tick)
{
    return (step * (uint32_t)TICKS_PER_STEP + (uint32_t)tick) / (uint32_t)time_division;
}

TimeDivision interval(const IntervalPattern& pattern, const uint32_t step, const uint8_t tick)
{
    uint32_t c = get_count(pattern.time_division, step, tick);
    return pattern.pattern[c % 16];
}

bool interval_hit(const TimeDivision time_division, const uint32_t step, const uint8_t tick)
{
    return (step * TICKS_PER_STEP + tick) % (uint8_t)time_division == 0;
}

bool interval_hit(const IntervalPattern& pattern, const uint32_t step, const uint8_t tick)
{
    return interval_hit(interval(pattern, step, tick), step, tick);
}

void randomize_interval(IntervalPattern& pattern, const IntervalProbs probs)
{
    for (int i = 0; i < 16; i++)
    {
        if (i % 2 == 0)
        {
            // Alles kan
            switch (distribution(probs.p_4, probs.p_8 * 2, probs.p_16 * 2, probs.p_32 * 2, probs.p_t8 * 2))
            {
            case 0: pattern.pattern[i] = TimeDivision::TIME_DIVISION_FOURTH; break;
            case 1: pattern.pattern[i] = TimeDivision::TIME_DIVISION_EIGHT; break;
            case 2: pattern.pattern[i] = TimeDivision::TIME_DIVISION_SIXTEENTH; break;
            case 3: pattern.pattern[i] = TimeDivision::TIME_DIVISION_THIRTYTWO; break;
            case 4: pattern.pattern[i] = TimeDivision::TIME_DIVISION_TRIPLE_EIGHT; break;
            }
        }
        else if (i == 16)
        {
            // Alles behalve triplets en kwarten
            switch (distribution(probs.p_8, probs.p_16, probs.p_32))
            {
            case 0: pattern.pattern[i] = TimeDivision::TIME_DIVISION_EIGHT; break;
            case 1: pattern.pattern[i] = TimeDivision::TIME_DIVISION_SIXTEENTH; break;
            case 2: pattern.pattern[i] = TimeDivision::TIME_DIVISION_THIRTYTWO; break;
            }
        }
        else
        {
            // alles behalve triplets
            switch (distribution(probs.p_4, probs.p_8 * 2, probs.p_16 * 2, probs.p_32 * 2))
            {
            case 0: pattern.pattern[i] = TimeDivision::TIME_DIVISION_FOURTH; break;
            case 1: pattern.pattern[i] = TimeDivision::TIME_DIVISION_EIGHT; break;
            case 2: pattern.pattern[i] = TimeDivision::TIME_DIVISION_SIXTEENTH; break;
            case 3: pattern.pattern[i] = TimeDivision::TIME_DIVISION_THIRTYTWO; break;
            }
        }

        if (pattern.pattern[i] == TimeDivision::TIME_DIVISION_FOURTH
            || pattern.pattern[i] == TimeDivision::TIME_DIVISION_TRIPLE_EIGHT)
        {
            pattern.pattern[i + 1] = pattern.pattern[i];
            i++;
        }
    }
}

void randomize_interval_lead(IntervalPattern& pattern)
{
    for (int i = 0; i < pattern.length; i++)
    {
        pattern.pattern[i] = TimeDivision::TIME_DIVISION_WHOLE;
    }
    for (int bar = 0; bar < 4; bar++)
    {
        // 50% chance for fill in each far
        if (randi(4) < 3)
        {
            // Starting fill of length 1 or two. Randomize position in bar.
            uint8_t fill_length = randi(1,3);
            uint8_t start_beat = randi(0, 5-fill_length);
            for (int beat = start_beat; beat < start_beat+fill_length; beat++)
            {
                TimeDivision time_division = randi(2) < 1 ? 
                    TimeDivision::TIME_DIVISION_SIXTEENTH : 
                    TimeDivision::TIME_DIVISION_EIGHT;
                pattern.pattern[bar * 4 + beat] = time_division;
            }
        }
    }
    pattern.time_division = TimeDivision::TIME_DIVISION_FOURTH;
}

void randomize_interval_hat(IntervalPattern& pattern)
{
    IntervalProbs probs = { 0,  20, 50, 15, 10 };
    probs.p_4 = 0;
    probs.p_8 = randi(30);
    probs.p_16 = 50;
    probs.p_32 = randi(15);
    probs.p_t8 = randi(15);
    randomize_interval(pattern, probs);
}