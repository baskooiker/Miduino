#pragma once

#include "core/defs.h"
#include "instruments/instrument_base.h"
#include "patterns/modulators.h"
#include "patterns/gate_patterns.h"
#include "instruments/parameters.h"
#include "instruments/drums/snare.h"

namespace Vleerhond
{
    const RandomParam sd_params[] = {
        {TB_SD_TUNE      ,   0,  16},
        {TB_SD_DTUNE     ,   0,  6},
        {TB_SD_SNAPPY    ,  32,  96},
        {TB_SD_SN_DECAY  , 110, 127},
        {TB_SD_TONE      ,   0,  32},
        {TB_SD_TONE_DECAY,   0,  16},
        {TB_SD_PITCH     ,   0,  32},
    };
    const uint8_t nr_sd_params = sizeof(sd_params) / sizeof(*sd_params);

    class TanzbarMid : public Snare
    {
    public:
        TanzbarMid(
            Modulators& modulators_ref,
            TimeStruct& time_ref) :
            Snare(modulators_ref, time_ref)
        {
            pitch = NOTE_TANZBAR_SD;
            settings.p_coef = 0;
            settings.p_off = 0;
        }

        void randomize()
        {
            Snare::randomize();
            Parameters::randomize_parameters(sd_params, nr_sd_params, MIDI_CC_CHANNEL_TANZBAR);
        }
    };
}