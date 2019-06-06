#pragma once

#include "defs.h"
#include "instrument_base.h"
#include "modulators.h"
#include "gate_patterns.h"
#include "parameters.h"
#include "percussion.h"

namespace Vleerhond
{
    const RandomParam cl_params[] = {

        {TB_CL_TUNE           ,  0, 127},
        {TB_CL_DECAY          , 80, 127}
    };
    const uint8_t nr_cl_params = sizeof(cl_params) / sizeof(*cl_params);

    class TanzbarCl : public Percussion
    {
    protected:
        ModulationReceiver cl_pitch;

    public:

        TanzbarCl(
            Modulators& modulators_ref,
            TimeStruct& time_ref) :
            Percussion(modulators_ref, time_ref),
            cl_pitch(modulators_ref)
        {
            midi_channel.set_channel(MIDI_CHANNEL_TANZBAR);
            pitch = NOTE_TANZBAR_CL;
        }

        void randomize()
        {
            ofLogNotice("tanzbar_perc", "randomize()");
            Percussion::randomize();
            Parameters::randomize_parameters(cl_params, nr_cl_params, MIDI_CC_CHANNEL_TANZBAR);

            // Modulators
            uint8_t range = Rand::randui8(128, 64);
            this->cl_pitch.randomize(range, 127 - range, .3);
        }

        void play()
        {
            uint8_t value = 0;
            if (this->cl_pitch.value(time, value))
            {
                MidiIO::send_cc(TB_CL_TUNE, value, MIDI_CC_CHANNEL_TANZBAR);
            }
            Percussion::play();
        }
    };
}