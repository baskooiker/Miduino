#pragma once

#include "cv_patterns.h"

class BassSettings
{
public:
    GatePatternAB accents;
    CvPatternAB pitches;
    CvPatternAB octaves;
    CvPatternAB variable_octaves;
    GatePatternAB slides;

    CvPatternAB probs;
    IntervalPattern int_pattern;
    GatePatternAB euclid_pattern;
    GatePatternAB low_pattern;

    uint8_t pitch_range;
    BassStyle style;
    CvPatternAB note_range_prob;
    uint8_t note_range_value;
    uint8_t octave_offset;
    uint8_t fugue_id;
    uint8_t density;

    bool kill;

    PitchStorage storage;

    BassSettings()
    {
        pitch_range = 0;
        style = BassStyle::BassLow;
        note_range_value = 0;
        density = 0;
        octave_offset = 2;
        kill = false;
    }
};
