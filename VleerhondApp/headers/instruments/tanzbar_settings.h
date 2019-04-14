#pragma once

#include "defs.h"
#include "modulators.h"
#include "mfb_503.h"
#include "timing_structs.h"
#include "interval_pattern.h"
#include "instrument_base.h"

#define MODULATOR_PROB (.3f)

const RandomParam tanzbar_low_params[] = {
    {TB_BD1_ATTACK   ,  0, 127},
    {TB_BD1_DECAY    , 32,  96},
    {TB_BD1_PITCH    , 80, 112},
    {TB_BD1_TUNE     , 32,  96},
    {TB_BD1_NOISE    ,  0,  64},
    {TB_BD1_FILTER   ,  0,  64},
    {TB_BD1_DIST     ,  0, 127},
    {TB_BD1_TRIGGER  ,  0, 127},

    {TB_BD2_DECAY    , 32,  96},
    {TB_BD2_TUNE     , 32,  96},
    {TB_BD2_TONE     , 32,  96}
};

const RandomParam tanzbar_mid_params[] = {
    {TB_SD_TUNE      ,  0, 127},
    {TB_SD_DTUNE     ,  0, 127},
    {TB_SD_SNAPPY    , 96, 127},
    {TB_SD_SN_DECAY  , 64, 127},
    {TB_SD_TONE      ,  0, 127},
    {TB_SD_TONE_DECAY,  0,  32},
    {TB_SD_PITCH     ,  0, 127},

    {TB_RS_TUNE      ,  0, 127},
    {TB_CP_DECAY     ,  0, 127},
    {TB_CP_FILTER    , 32, 127},
    {TB_CP_ATTACK    ,  0, 127},
    {TB_CP_TRIGGER   ,  0, 127}
};

const RandomParam tanzbar_hi_params[] = {
    {TB_OH_DECAY,  0,  96},
    {TB_HH_TUNE ,  0, 127},
    {TB_HH_DECAY,  0, 127},

    {TB_CY_DECAY,  64, 127},
    {TB_CY_TONE ,   0, 127},
    {TB_CY_TUNE ,   0, 127}
};

const RandomParam tanzbar_perc_params[] = {

    {TB_CL_TUNE           ,  0, 127},
    {TB_CL_DECAY          ,  0, 127},
    {TB_HTC_TUNE          ,  0, 127},
    {TB_HTC_DECAY         , 64,  96},
    {TB_HTC_NOISE_ON_OFF  ,  0, 127},
    {TB_HTC_TOM_CONGA     ,  0, 127},
    {TB_MTC_TUNE          ,  0, 127},
    {TB_MTC_DECAY         , 64,  96},
    {TB_MTC_NOISE_ON_OFF  ,  0, 127},
    {TB_MTC_TOM_CONGA     ,  0, 127},
    {TB_LTC_TUNE          ,  0, 127},
    {TB_LTC_DECAY         , 64,  96},
    {TB_LTC_NOISE_ON_OFF  ,  0, 127},
    {TB_LTC_TOM_CONGA     ,  0, 127},
    {TB_TOM_NOISE         ,  0, 127},
    {TB_CB_Tune           ,  0, 127},
    {TB_CB_Decay          ,  0, 127},
    {TB_MA_Decay          ,  0, 127},
};
const uint8_t nr_tanzbar_low_params = sizeof(tanzbar_low_params) / sizeof(RandomParam);
const uint8_t nr_tanzbar_mid_params = sizeof(tanzbar_mid_params) / sizeof(RandomParam);
const uint8_t nr_tanzbar_perc_params = sizeof(tanzbar_perc_params) / sizeof(RandomParam);
const uint8_t nr_tanzbar_hi_params = sizeof(tanzbar_hi_params) / sizeof(RandomParam);

class TanzbarModulators
{
public:
    ModulationReceiver cl_pitch;
    ModulationReceiver ma_dec;
    ModulationReceiver rs_tune;
    ModulationReceiver cb_tune;
    ModulationReceiver cp_trig;
    ModulationReceiver cy_tune;
    ModulationReceiver cy_vel;
    ModulationReceiver tom_vel;

    TanzbarModulators(Modulators& modulators) :
        cl_pitch(modulators),
        ma_dec(modulators),
        rs_tune(modulators),
        cb_tune(modulators),
        cp_trig(modulators),
        cy_tune(modulators),
        cy_vel(modulators),
        tom_vel(modulators)
    {

    }

    void randomize()
    {
        ofLogNotice("TanzbarModulators", "randomize");
        uint8_t range = Rand::randui8(128, 64);
        this->cl_pitch.randomize(range, 127 - range, MODULATOR_PROB);

        this->ma_dec.randomize(Rand::randui8(128, 64), 0);

        range = Rand::randui8(128);
        this->rs_tune.randomize(range, 127 - range, MODULATOR_PROB);

        range = Rand::randui8(128);
        this->cb_tune.randomize(range, 127 - range, MODULATOR_PROB);

        range = Rand::randui8(128);
        this->cp_trig.randomize(range, 127 - range, MODULATOR_PROB);

        range = Rand::randui8(128);
        this->cy_tune.randomize(range, 127 - range, MODULATOR_PROB);

        range = Rand::randui8(16, 64);
        this->cy_vel.randomize(range, 127 - range);

        range = Rand::randui8(16, 64);
        this->tom_vel.randomize(range, 127 - range);
    }
};

class Tanzbar
{
public:
    static void randomize_parameters(const RandomParam* list, const uint8_t length)
    {
        for (int i = 0; i < length; i++)
        {
            MidiIO::send_cc(list[i].note,
                Rand::randui8(list[i].min, list[i].max),
                MIDI_CC_CHANNEL_TANZBAR);
        }
    }

};
