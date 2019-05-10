#pragma once

#include "defs.h"
#include "instrument_base.h"
#include "modulators.h"
#include "gate_patterns.h"
#include "parameters.h"
#include "percussion.h"

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
            storage.set_channel(MIDI_CHANNEL_TANZBAR);
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

        void play()
        {
            uint8_t value = 0;
            if (this->cb_tune.value(time, value))
            {
                MidiIO::send_cc(TB_CB_Tune, value, MIDI_CC_CHANNEL_TANZBAR);
            }
            Percussion::play();
        }
    };
}