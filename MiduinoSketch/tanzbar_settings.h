#pragma once

#include "defs.h"
#include "modulators.h"

#define MODULATOR_PROB (.2f)

class TanzbarModulators
{
public:
    ModulationReceiver cl_pitch;
    ModulationReceiver ma_dec;
    ModulationReceiver rs_tune;
    ModulationReceiver cb_tune;
    ModulationReceiver cp_trig;
    ModulationReceiver cy_tune;

    void randomize()
    {
        uint8_t range = randui8(128, 64);
        this->cl_pitch.randomize(range, 127 - range, MODULATOR_PROB);

        this->ma_dec.randomize(randui8(128, 64), 0);

        range = randui8(128);
        this->rs_tune.randomize(range, 127 - range, MODULATOR_PROB);

        range = randui8(128);
        this->cb_tune.randomize(range, 127 - range, MODULATOR_PROB);

        range = randui8(128);
        this->cp_trig.randomize(range, 127 - range, MODULATOR_PROB);

        range = randui8(128);
        this->cy_tune.randomize(range, 127 - range, MODULATOR_PROB);
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

