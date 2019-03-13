#pragma once

#include "application_data.h"

void randomize_harmony(ApplicationData& data)
{
    // Set const pattern
    data.harmony.const_pattern.set_all(0);
    data.harmony.const_pattern.time_division = TimeDivision::Sixteenth;

    // Set high pattern
    set_chord_pattern(data.harmony.high_pattern);
    //set_chord_pattern_ab(data.harmony.high_pattern);
    //if (distribution(16, 16))
    //{
    //    set_ab_pattern_low(data.harmony.high_pattern.abPattern);
    //}
    //else
    //{
    //    set_ab_pattern_high(data.harmony.high_pattern.abPattern);
    //}

    switch (distribution(10, 10, 10))
    {
    case 0: data.harmony.high_pattern.time_division = TimeDivision::Quarter; break;
    case 1: data.harmony.high_pattern.time_division = TimeDivision::Half; break;
    case 2: data.harmony.high_pattern.time_division = TimeDivision::Whole; break;
    }
}

uint8_t get_chord_step(const HarmonyStruct& harmony, const TimeStruct& time)
{
    switch (harmony.type)
    {
    case HarmonyType::HarmonyConst:
        return harmony.const_pattern.value(time);
    case HarmonyType::HarmonyHigh:
        return harmony.high_pattern.value(time);
    }
    return 0;
}
