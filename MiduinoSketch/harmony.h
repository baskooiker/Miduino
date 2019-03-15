#pragma once

#include "application_data.h"

//void randomize_harmony(ApplicationData& data)
//{
//    // Set const pattern
//    switch (distribution(64, 16))
//    {
//    case 0: data.harmony.const_value = 0; break;
//    case 1: data.harmony.const_value = 4; break;
//    }
//
//    // Set high pattern
//    switch (distribution(64, 16))
//    {
//    case 0: set_chord_pattern(data.harmony.high_pattern, 0); break;
//    case 1: set_chord_pattern(data.harmony.high_pattern, 4); break;
//    }
//
//    switch (distribution(10, 10, 10))
//    {
//    case 0: data.harmony.high_pattern.time_division = TimeDivision::Quarter; break;
//    case 1: data.harmony.high_pattern.time_division = TimeDivision::Half; break;
//    case 2: data.harmony.high_pattern.time_division = TimeDivision::Whole; break;
//    }
//}
