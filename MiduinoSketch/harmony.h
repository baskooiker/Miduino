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
    data.harmony.high_pattern.time_division = TimeDivision::Sixteenth;
}

uint8_t get_chord_step(const HarmonyStruct& harmony, const uint32_t step, const uint8_t tick)
{
    switch (harmony.type)
    {
    case HarmonyType::HarmonyConst:
        return cv(harmony.const_pattern, step);
    case HarmonyType::HarmonyLow:
        return cv(harmony.low_pattern, step);
    case HarmonyType::HarmonyHigh:
        return cv(harmony.high_pattern, step);
    }
    return 0;
}
