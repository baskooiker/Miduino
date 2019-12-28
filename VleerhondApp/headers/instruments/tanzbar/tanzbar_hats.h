#pragma once

#include "hats.h"

namespace Vleerhond
{
    const RandomParam tanzbar_hats_params[] = {
        //{TB_OH_DECAY,  0,  96},
        {TB_HH_TUNE ,  0, 127},
        //{TB_HH_DECAY,  0, 127},
    };
    const uint8_t nr_of_tanzbar_hats_params = sizeof(tanzbar_hats_params) / sizeof(*tanzbar_hats_params);

    class TanzbarHats : public Hats
    {
    protected:
        ModulationReceiver tune_mod;
        ModulationReceiver oh_dec;

    public:
        GatePatternAB hh_pattern;
        GatePatternAB oh_pattern;

        //IntervalPattern hat_int_pattern;
        //CvPatternAB hat_velocity;

        //HatClosedStyle hat_closed_style;

        TanzbarHats(
            Modulators& modulators,
            TimeStruct& time) :
            Hats(modulators, time, MIDI_CHANNEL_TANZBAR),
            tune_mod(modulators),
            oh_dec(modulators)
        {
            //hat_closed_style = HatClosedStyle::HatClosedRegular;
            randomize();

            pitch_open = NOTE_TANZBAR_OH;
            pitch_closed = NOTE_TANZBAR_HH;

            settings.p_euclid = 0;
            settings.p_drop = 0;
        }

        void randomize()
        {
            ofLogNotice("tanzbar_hi", "randomize()");
            Hats::randomize();

            Parameters::randomize_parameters(tanzbar_hats_params, nr_of_tanzbar_hats_params, MIDI_CC_CHANNEL_TANZBAR);

            // Modulators
            {
                uint8_t range = Rand::randui8(16, 96);
                uint8_t off = Rand::randui8(127 - range);
                this->tune_mod.randomize(range, off, .33);
            }

            {
                uint8_t range = Rand::randui8(16, 64);
                uint8_t off = Rand::randui8(96 - range);
                this->oh_dec.randomize(range, off, .33);
            }

        }

        bool play()
        {
            if (this->kill)
            {
                return false;
            }

            uint8_t value = 0;
            if (tune_mod.value(time, value))
            {
                MidiIO::send_cc(TB_HH_TUNE, value, MIDI_CC_CHANNEL_TANZBAR);
            }
            return Hats::play();
        }
    };
}