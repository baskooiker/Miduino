#pragma once

#include "defs.h"
#include "instrument_base.h"
#include "modulators.h"
#include "gate_patterns.h"
#include "tanzbar_settings.h"

namespace Vleerhond
{
    const RandomParam cb_params[] = {
        {TB_CB_Tune           ,  0, 127},
        {TB_CB_Decay          ,  0, 127},
    };
    const uint8_t nr_cb_params = sizeof(cb_params) / sizeof(*cb_params);

    class TanzbarCb : public InstrumentBase
    {
    protected:
        MicroTimingStruct cb_timing;

        ModulationReceiver cb_tune;

        GatePatternAB cb_pattern;

    public:

        TanzbarCb(
            Modulators& modulators_ref,
            TimeStruct& time_ref) :
            InstrumentBase(time_ref, true),
            cb_tune(modulators_ref)
        {
            storage.set_channel(MIDI_CHANNEL_TANZBAR);
        }

        void randomize()
        {
            ofLogNotice("tanzbar_perc", "randomize()");
            last_randomized_time = millis();

            Tanzbar::randomize_parameters(cb_params, nr_cb_params);

            this->cb_pattern.randomize();

            // Modulators
            uint8_t range = Rand::randui8(128);
            this->cb_tune.randomize(range, 127 - range, .3);

            this->cb_timing.randomize();
        }

        void play()
        {
            // Play cowbell
            if (this->cb_pattern.gate(time) && !kill)
            {
                uint8_t value = 0;
                if (this->cb_tune.value(time, value))
                {
                    MidiIO::send_cc(TB_CB_Tune, value, MIDI_CC_CHANNEL_TANZBAR);
                }
                this->storage.note_on(
                    NoteStruct(NOTE_TANZBAR_CB, 64),
                    time.get_shuffle_delay(this->cb_timing)
                );
            }
        }
    };
}