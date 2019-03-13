#pragma once

#include "cv_patterns.h"

class Mfb503Settings
{
public:
    GatePatternAB bd_pattern;
    GatePatternAB sd_pattern;
    GatePatternAB hh_pattern;
    GatePatternAB oh_pattern;
    GatePatternAB cy_pattern;
    CvPatternAB tom_pattern;

    IntervalPattern hat_int_pattern;
    CvPatternAB hat_velocity;

    uint8_t bd_decay;
    bool play_pitch_bd;

    uint8_t volume_cy;

    uint8_t volume_tom;
    uint8_t nr_toms;
    uint8_t toms_offset;
    GatePatternAB tom_mask;

    HatClosedStyle hat_closed_style;
    uint8_t closed_hat_note;

    bool kill_low;
    bool kill_mid;
    bool kill_perc;
    bool kill_hats;
    bool drum_fill;
    uint8_t snare_roll;
    uint8_t bd_decay_factor;

    PitchStorage storage;

    Mfb503Settings()
    {
        hat_closed_style = HatClosedStyle::HatClosedRegular;
        volume_cy = 0;
        volume_tom = 0;
        kill_hats = false;
        closed_hat_note = NOTE_503_HH_1;
    }
};
