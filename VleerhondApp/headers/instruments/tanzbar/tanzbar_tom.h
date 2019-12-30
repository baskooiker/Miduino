#pragma once

#include "core/defs.h"
#include "instrument_base.h"
#include "patterns/modulators.h"
#include "patterns/gate_patterns.h"
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
        ModulationReceiver low_pitch;
        ModulationReceiver mid_pitch;
        ModulationReceiver high_pitch;

    public:

        TanzbarTom(
            Modulators& modulators,
            TimeStruct& time) :
            Toms(modulators, time, MIDI_CHANNEL_TANZBAR),
            low_pitch(modulators),
            mid_pitch(modulators),
            high_pitch(modulators)
        {
        }

        void randomize()
        {
            ofLogNotice("tanzbar_perc", "randomize()");

            Toms::randomize();

            Parameters::randomize_parameters(tom_params, nr_tom_params, MIDI_CC_CHANNEL_TANZBAR);

            this->pitches.clear();
            switch (Rand::distribution(32, 32))
            {
            case 0:
                pitches.push_back(NOTE_TANZBAR_LT);
                break;
            case 1:
                pitches.push_back(NOTE_TANZBAR_LC);
                break;
            }
            switch (Rand::distribution(32, 32))
            {
            case 0:
                pitches.push_back(NOTE_TANZBAR_MT);
                break;
            case 1:
                pitches.push_back(NOTE_TANZBAR_MC);
                break;
            }
            switch (Rand::distribution(32, 32))
            {
            case 0:
                pitches.push_back(NOTE_TANZBAR_HT);
                break;
            case 1:
                pitches.push_back(NOTE_TANZBAR_HC);
                break;
            }

            {
                uint8_t range = Rand::randui8(64, 128);
                low_pitch.randomize(range, Rand::randui8(127 - range), .5);
            }
            {
                uint8_t range = Rand::randui8(64, 128);
                mid_pitch.randomize(range, Rand::randui8(127 - range), .5);
            }
            {
                uint8_t range = Rand::randui8(64, 128);
                high_pitch.randomize(range, Rand::randui8(127 - range), .5);
            }
        }

        bool play()
        {
            uint8_t value = 0;
            if (low_pitch.value(time, value))
            {
                MidiIO::send_cc(TB_LTC_TUNE, value, MIDI_CC_CHANNEL_TANZBAR);
            }
            if (mid_pitch.value(time, value))
            {
                MidiIO::send_cc(TB_MTC_TUNE, value, MIDI_CC_CHANNEL_TANZBAR);
            }
            if (high_pitch.value(time, value))
            {
                MidiIO::send_cc(TB_HTC_TUNE, value, MIDI_CC_CHANNEL_TANZBAR);
            }
            return Toms::play();
        }
    };
}