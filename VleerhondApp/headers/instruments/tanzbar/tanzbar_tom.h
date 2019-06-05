#pragma once

#include "defs.h"
#include "instrument_base.h"
#include "modulators.h"
#include "gate_patterns.h"
#include "parameters.h"
#include "toms.h"

namespace Vleerhond
{
    const RandomParam tom_params[] = {

        {TB_HTC_TUNE          , 64, 127},
        {TB_HTC_DECAY         , 64,  96},
        {TB_HTC_NOISE_ON_OFF  ,  0, 127},
        {TB_HTC_TOM_CONGA     ,  0, 127},
        {TB_MTC_TUNE          , 64, 127},
        {TB_MTC_DECAY         , 64,  96},
        {TB_MTC_NOISE_ON_OFF  ,  0, 127},
        {TB_MTC_TOM_CONGA     ,  0, 127},
        {TB_LTC_TUNE          , 64, 127},
        {TB_LTC_DECAY         , 64,  96},
        {TB_LTC_NOISE_ON_OFF  ,  0, 127},
        {TB_LTC_TOM_CONGA     ,  0, 127},
        {TB_TOM_NOISE         ,  0, 127},
    };
    const uint8_t nr_tom_params = sizeof(tom_params) / sizeof(*tom_params);

    class TanzbarTom : public Toms
    {
    public:

        TanzbarTom(
            Modulators& modulators,
            TimeStruct& time) :
            Toms(modulators, time)
        {
            storage.set_channel(MIDI_CHANNEL_TANZBAR);
        }

        void randomize()
        {
            ofLogNotice("tanzbar_perc", "randomize()");

            Toms::randomize();

            Parameters::randomize_parameters(tom_params, nr_tom_params, MIDI_CC_CHANNEL_TANZBAR);

            this->pitches.clear();
            switch (Rand::distribution(0, 32))
            {
            case 0:
                pitches.push_back(NOTE_TANZBAR_LT);
                pitches.push_back(NOTE_TANZBAR_MT);
                pitches.push_back(NOTE_TANZBAR_HT);
                break;
            case 1:
                pitches.push_back(NOTE_TANZBAR_LC);
                pitches.push_back(NOTE_TANZBAR_MC);
                pitches.push_back(NOTE_TANZBAR_HC);
                break;
            }
        }
    };
}