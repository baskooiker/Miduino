#pragma once

#include "defs.hpp"
#include "instrument_base.hpp"
#include "modulators.hpp"
#include "gate_patterns.hpp"
#include "parameters.hpp"
#include "percussion.hpp"

namespace Vleerhond
{
    const RandomParam cb_params[] = {
        {TB_CB_Tune           ,  0, 127},
        {TB_CB_Decay          ,  0, 127},
    };
    const uint8_t nr_cb_params = sizeof(cb_params) / sizeof(*cb_params);

    class TanzbarCb : public Percussion
    {
    protected:
        ModulationReceiver cb_tune;

    public:

        TanzbarCb(
            Modulators& modulators_ref,
            TimeStruct& time_ref) :
            Percussion(modulators_ref, time_ref),
            cb_tune(modulators_ref)
        {
            midi_channel.set_channel(MIDI_CHANNEL_TANZBAR);
            pitch = NOTE_TANZBAR_CB;
        }

        void randomize()
        {
            ofLogNotice("tanzbar_perc", "randomize()");
            Parameters::randomize_parameters(cb_params, nr_cb_params, MIDI_CC_CHANNEL_TANZBAR);
            Percussion::randomize();

            // Modulators
            uint8_t range = Rand::randui8(128);
            this->cb_tune.randomize(range, 127 - range, .3);
        }

        bool play()
        {
            uint8_t value = 0;
            if (this->cb_tune.value(time, value))
            {
                MidiIO::send_cc(TB_CB_Tune, value, MIDI_CC_CHANNEL_TANZBAR);
            }
            return Percussion::play();
        }
    };
}