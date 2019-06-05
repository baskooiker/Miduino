#pragma once

#include "defs.h"
#include "instrument_base.h"
#include "modulators.h"
#include "gate_patterns.h"
#include "parameters.h"
#include "snare.h"

namespace Vleerhond
{
    const RandomParam cp_params[] = {
        {TB_RS_TUNE      ,  0, 127},
        {TB_CP_DECAY     ,  0, 127},
        {TB_CP_FILTER    , 32, 127},
        {TB_CP_ATTACK    ,  0, 127},
        {TB_CP_TRIGGER   ,  0,  64}
    };
    const uint8_t nr_cp_params = sizeof(cp_params) / sizeof(*cp_params);

    class TanzbarCp : public Snare
    {
    protected:
        ModulationReceiver cp_trig;
        ModulationReceiver filter_mod;

    public:
        TanzbarCp(
            Modulators& modulators_ref,
            TimeStruct& time_ref) :
            Snare(modulators_ref, time_ref),
            cp_trig(modulators_ref),
            filter_mod(modulators_ref)
        {
            storage.set_channel(MIDI_CHANNEL_TANZBAR);
            randomize();
            pitch = NOTE_TANZBAR_CP;
        }

        void randomize()
        {
            Snare::randomize();
            Parameters::randomize_parameters(cp_params, nr_cp_params, MIDI_CC_CHANNEL_TANZBAR);

            // Modulators
            uint8_t range = Rand::randui8(128);
            this->cp_trig.randomize(range, 127 - range, .3);

            {
                uint8_t range = Rand::randui8(128);
                this->filter_mod.randomize(range, 127 - range, .3);
            }
        }

        void play()
        {
            uint8_t value = 0;
            if (this->cp_trig.value(time, value))
            {
                MidiIO::send_cc(TB_CP_TRIGGER, value, MIDI_CC_CHANNEL_TANZBAR);
            }
            if (this->filter_mod.value(time, value))
            {
                MidiIO::send_cc(TB_CP_FILTER, value, MIDI_CC_CHANNEL_TANZBAR);
            }
            Snare::play();
        }
    };
}