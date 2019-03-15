#pragma once

#include "defs.h"
#include "scale.h"

class HarmonyStruct {
public:
    uint8_t const_value;
    CvPattern16 high_pattern;
    HarmonyType type;
    Scale scale;

    HarmonyStruct()
    {
        type = HarmonyType::HarmonyConst;
    }

    uint8_t get_chord_step(const TimeStruct& time) const
    {
        switch (this->type)
        {
        default:
        case HarmonyType::HarmonyConst:
            return this->const_value;
        case HarmonyType::HarmonyHigh:
            return this->high_pattern.value(time);
        }
    }
    void randomize()
    {
        // Set const pattern
        switch (distribution(64, 16))
        {
        case 0: this->const_value = 0; break;
        case 1: this->const_value = 4; break;
        }

        // Set high pattern
        switch (distribution(64, 16))
        {
        case 0: set_chord_pattern(this->high_pattern, this->scale, 0); break;
        case 1: set_chord_pattern(this->high_pattern, this->scale, 4); break;
        }

        switch (distribution(10, 10, 10))
        {
        case 0: this->high_pattern.time_division = TimeDivision::Quarter; break;
        case 1: this->high_pattern.time_division = TimeDivision::Half; break;
        case 2: this->high_pattern.time_division = TimeDivision::Whole; break;
        }
    }

    void switch_const_chord()
    {
        uint8_t options[8] = { 0 };
        uint8_t length = 0;
        this->scale.get_available_chords_indices(options, length);
        remove(this->const_value, options, length);
        remove(0, options, length);
        this->const_value = options[randui8(length)];
    }

};
