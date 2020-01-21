#pragma once

#include "ofMain.h"

#include "core/defs.h"

#include "harmony/scale.h"
#include "harmony/chords.h"
#include "patterns/cv_patterns.h"

namespace Vleerhond
{
    class HarmonyStruct {
    public:
        uint8_t const_value;
        CvPattern16 high_pattern;
        HarmonyType type;
        Scale scale;

        HarmonyStruct();
        uint8_t get_chord_step(const TimeStruct& time) const;
        void randomize();
        void setHighPattern(const uint8_t start_chord, const bool long_pattern);
        void switch_const_chord();
    };
}