#pragma once

#include "ofMain.h"

#include "defs.h"
#include "scale.h"
#include "chords.h"
#include "patterns/cv_patterns.h"

const std::string HARMONY = "harmony";

namespace Vleerhond
{
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
            ofLogNotice(HARMONY, "randomize()");
            // Set const pattern
            switch (Rand::distribution(64, 16))
            {
            case 0: this->const_value = 0; break;
            case 1: this->const_value = 4; break;
            }

            // Set high pattern
            switch (Rand::distribution(64, 16))
            {
            case 0: this->high_pattern.set_chord_pattern(this->scale, 0); break;
            case 1: this->high_pattern.set_chord_pattern(this->scale, 4); break;
            }

            switch (Rand::distribution(10, 10))
            {
            case 0: this->high_pattern.time_division = TimeDivision::Quarter; break;
            case 1: this->high_pattern.time_division = TimeDivision::Half; break;
            }
        }

        void switch_const_chord()
        {
            uint8_t options[8] = { 0 };
            uint8_t length = 0;
            this->scale.get_available_chords_indices(options, length);
            Utils::remove(this->const_value, options, length);
            Utils::remove(0, options, length);
            this->const_value = options[Rand::randui8(length)];
        }
    };
}