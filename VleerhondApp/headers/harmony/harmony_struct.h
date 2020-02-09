#pragma once

#include "ofMain.h"

#include "core/defs.h"

#include "harmony/scale.h"
#include "harmony/chords.h"
#include "patterns/cv_patterns.h"

namespace Vleerhond
{
    class HarmonyStruct {
    private:
        HarmonyType type = HarmonyType::Const;
        uint8_t const_value = 0;

    public:
        CvPattern16 low_tonic_pattern;
        CvPattern16 low_dominant_pattern;
        CvPattern16 high_tonic_pattern;
        CvPattern16 high_dominant_pattern;

        Scale scale;

        HarmonyStruct();
        uint8_t get_chord_step(const TimeStruct& time) const;
        void randomize();
        void switch_const_chord();
        uint16_t getPatternLength(HarmonyType type);
        void HarmonyStruct::setType(HarmonyType type);
        void setTonic();

    private:
        void randomizeHighPattern(CvPattern16& high_pattern, const uint8_t start_chord, const bool long_pattern);
    };
}