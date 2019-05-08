#pragma once

#include "defs.h"
#include "instrument_base.h"
#include "modulators.h"
#include "gate_patterns.h"
#include "parameters.h"

namespace Vleerhond
{
    const RandomParam cp_params[] = {
        {TB_RS_TUNE      ,  0, 127},
        {TB_CP_DECAY     ,  0, 127},
        {TB_CP_FILTER    , 32, 127},
        {TB_CP_ATTACK    ,  0, 127},
        {TB_CP_TRIGGER   ,  0, 127}
    };
    const uint8_t nr_cp_params = sizeof(cp_params) / sizeof(*cp_params);

    class TanzbarCp : public InstrumentBase
    {
    protected:
        GatePatternAB cp_pattern;

        MicroTimingStruct cp_timing;

        ModulationReceiver cp_trig;

    public:
        TanzbarCp(
            Modulators& modulators_ref,
            TimeStruct& time_ref) :
            InstrumentBase(time_ref, true),
            cp_trig(modulators_ref)
        {
            storage.set_channel(MIDI_CHANNEL_TANZBAR);
        }

        void randomize()
        {
            last_randomized_time = millis();

            Parameters::randomize_parameters(cp_params, nr_cp_params, MIDI_CC_CHANNEL_TANZBAR);

            this->cp_pattern.set_coef_snare_pattern();

            // Modulators
            uint8_t range = Rand::randui8(128);
            this->cp_trig.randomize(range, 127 - range, .3);

            // Timing
            this->cp_timing.randomize();
        }

        void play()
        {
            // Play clap
            if (this->cp_pattern.gate(time) && !this->kill)
            {
                uint8_t value = 0;
                if (this->cp_trig.value(time, value))
                {
                    MidiIO::send_cc(TB_CP_TRIGGER, value, MIDI_CC_CHANNEL_TANZBAR);
                }
                this->storage.note_on(
                    NoteStruct(NOTE_TANZBAR_CP, 64),
                    time.get_shuffle_delay(this->cp_timing)
                );
            }
        }
    };
}