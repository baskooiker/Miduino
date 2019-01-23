#pragma once

#include "cv.h"

void randomize_harmony(ApplicationData& data)
{
    // Set const pattern
    set_all(data.harmony.const_pattern, 0);
    data.harmony.const_pattern.time_division = TimeDivision::TIME_DIVISION_SIXTEENTH;

    // Set low pattern
    set_chord_pattern_ab(data.harmony.low_pattern);
    if (randf() < .5)
    {
        set_ab_pattern_low(data.harmony.low_pattern.abPattern);
        data.harmony.low_pattern.time_division = TimeDivision::TIME_DIVISION_SIXTEENTH;
    }
    else
    {
        set_ab_pattern_high(data.harmony.low_pattern.abPattern);
        data.harmony.low_pattern.time_division = TimeDivision::TIME_DIVISION_EIGHT;
    }

    // Set high pattern
    set_chord_pattern_ab(data.harmony.high_pattern);
    set_ab_pattern_high(data.harmony.high_pattern.abPattern);
    data.harmony.high_pattern.time_division = TimeDivision::TIME_DIVISION_SIXTEENTH;
}

uint8_t get_chord_step(const ApplicationData& data)
{
    switch (data.harmony.type)
    {
    case HarmonyType::Const: 
        return cv(data.harmony.const_pattern, data.step);
    case HarmonyType::Low:
        return cv(data.harmony.low_pattern, data.step);
    case HarmonyType::High:
        return cv(data.harmony.high_pattern, data.step);
    }
    return 0;
}
