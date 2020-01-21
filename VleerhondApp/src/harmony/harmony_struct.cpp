#include "harmony/harmony_struct.h"
#include "ofMain.h"

#include "core/defs.h"
#include "harmony/scale.h"
#include "harmony/chords.h"
#include "patterns/cv_patterns.h"
#include "utils/utils.h"

const std::string HARMONY = "harmony";

namespace Vleerhond
{
    HarmonyStruct::HarmonyStruct()
    {
        type = HarmonyType::HarmonyConst;
    }

    uint8_t HarmonyStruct::get_chord_step(const TimeStruct& time) const
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

    void HarmonyStruct::randomize()
    {
        ofLogNotice(HARMONY, "randomize()");
        // Set const pattern
        switch (Rand::distribution(64, 16))
        {
        case 0: this->const_value = 0; break;
        case 1: this->const_value = 4; break;
        }

        // Set high pattern
        const uint8_t start_chord = Rand::distribution(64, 16) ? 0 : 4;
        const bool long_pattern = Rand::distribution(16, 16) == 1;
        this->setHighPattern(start_chord, long_pattern);
    }

    void HarmonyStruct::setHighPattern(const uint8_t start_chord, const bool long_pattern)
    {
        this->type = HarmonyType::HarmonyHigh;

        this->high_pattern.set_chord_pattern(this->scale, start_chord, long_pattern);

        switch (Rand::distribution(10, 10))
        {
        case 0: this->high_pattern.time_division = TimeDivision::Quarter; break;
        case 1: this->high_pattern.time_division = TimeDivision::Half; break;
        }
    }

    void HarmonyStruct::switch_const_chord()
    {
        std::vector<uint8_t> options = this->scale.getAvailableChordsIndices();
        options.erase(std::remove(options.begin(), options.end(), const_value), options.end());
        options.erase(std::remove(options.begin(), options.end(), 0), options.end());
        this->const_value = options[Rand::randui8(options.size())];
    }
}