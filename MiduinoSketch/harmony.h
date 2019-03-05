#pragma once

#include "cv.h"

void randomize_harmony(ApplicationData& data)
{
    // Set const pattern
    set_all(data.harmony.const_pattern, 0);
    data.harmony.const_pattern.time_division = TimeDivision::Sixteenth;

    // Set low pattern
    set_chord_pattern_ab(data.harmony.low_pattern);
    if (randf() < .5)
    {
        set_ab_pattern_low(data.harmony.low_pattern.abPattern);
        data.harmony.low_pattern.time_division = TimeDivision::Sixteenth;
    }
    else
    {
        set_ab_pattern_high(data.harmony.low_pattern.abPattern);
        data.harmony.low_pattern.time_division = TimeDivision::Eight;
    }

    // Set high pattern
    set_chord_pattern_ab(data.harmony.high_pattern);
    set_ab_pattern_high(data.harmony.high_pattern.abPattern);
    switch (distribution(10, 10))
    {
    case 0: data.harmony.high_pattern.time_division = TimeDivision::Sixteenth; break;
    case 1: data.harmony.high_pattern.time_division = TimeDivision::Eight; break;
    }
}

uint8_t get_chord_step(const HarmonyStruct& harmony, const TimeStruct& time)
{
    switch (harmony.type)
    {
    case HarmonyType::HarmonyConst:
        return cv(harmony.const_pattern, time);
    case HarmonyType::HarmonyLow:
        return cv(harmony.low_pattern, time);
    case HarmonyType::HarmonyHigh:
        return cv(harmony.high_pattern, time);
    }
    return 0;
}
