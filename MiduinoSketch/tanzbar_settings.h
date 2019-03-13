#pragma once

#include "defs.h"
#include "modulators.h"

class TanzbarModulators
{
public:
    ModulationReceiver cl_pitch;
    ModulationReceiver ma_dec;

    void randomize()
    {
        uint8_t cl_range = randui8(128, 64);
        this->cl_pitch.randomize(cl_range, 127 - cl_range, .25);

        this->ma_dec.randomize(randui8(128, 64), 0);

    }
};

class TanzbarSettings 
{
public:
    GatePatternAB bd_pattern;
    GatePatternAB sd_pattern;
    GatePatternAB rs_pattern;
    GatePatternAB cp_pattern;
    GatePatternAB hh_pattern;
    GatePatternAB oh_pattern;
    GatePatternAB cy_pattern;
    GatePatternAB cl_pattern;
    GatePatternAB cb_pattern;
    CvPatternAB tom_pattern;
    CvPatternAB ma_pattern;

    IntervalPattern hat_int_pattern;
    CvPatternAB hat_velocity;

    uint8_t toms_offset;
    PercussionType percussion_type;
    GatePatternAB tom_mask;

    HatClosedStyle hat_closed_style;

    bool kill_low;
    bool kill_mid;
    bool kill_perc;
    bool kill_hats;
    bool drum_fill;
    uint8_t snare_roll;

    TanzbarModulators modulators;
    TanzbarTimeSettings time_settings;

    PitchStorage storage;

    TanzbarSettings()
    {
        hat_closed_style = HatClosedStyle::HatClosedRegular;
        kill_hats = false;
        percussion_type = PercussionType::PercussionToms;
    }
};

