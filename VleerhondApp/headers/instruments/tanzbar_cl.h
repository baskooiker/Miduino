#pragma once

#include "defs.h"
#include "instrument_base.h"
#include "modulators.h"
#include "gate_patterns.h"
#include "parameters.h"

namespace Vleerhond
{
    const RandomParam cl_params[] = {

        {TB_CL_TUNE           ,  0, 127},
        {TB_CL_DECAY          , 80, 127}
    };
    const uint8_t nr_cl_params = sizeof(cl_params) / sizeof(*cl_params);

    class TanzbarCl : public InstrumentBase
    {
    protected:
        MicroTimingStruct cl_timing;
        ModulationReceiver cl_pitch;
        GatePatternAB cl_pattern;

    public:

        TanzbarCl(
            Modulators& modulators_ref,
            TimeStruct& time_ref) :
            InstrumentBase(time_ref, true),
            cl_pitch(modulators_ref)
        {
            storage.set_channel(MIDI_CHANNEL_TANZBAR);
        }

        void randomize()
        {
            ofLogNotice("tanzbar_perc", "randomize()");
            last_randomized_time = millis();

            Parameters::randomize_parameters(cl_params, nr_cl_params, MIDI_CC_CHANNEL_TANZBAR);

            this->cl_pattern.randomize();

            // Modulators
            uint8_t range = Rand::randui8(128, 64);
            this->cl_pitch.randomize(range, 127 - range, .3);

            this->cl_timing.randomize();
        }

        void play()
        {
            // Play clave
            if (this->cl_pattern.gate(time))
            {
                uint8_t value = 0;
                if (this->cl_pitch.value(time, value))
                {
                    MidiIO::send_cc(TB_CL_TUNE, value, MIDI_CC_CHANNEL_TANZBAR);
                }
                this->storage.note_on(
                    NoteStruct(NOTE_TANZBAR_CL, 64),
                    time.get_shuffle_delay(this->cl_timing)
                );
            }
        }
    };
}