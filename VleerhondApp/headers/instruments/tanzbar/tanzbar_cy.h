#pragma once

#include "instrument_base.h"
#include "core/timing_structs.h"
#include "core/time_struct.h"
#include "patterns/modulators.h"
#include "patterns/gate_patterns.h"
#include "cymbal.h"
#include "midi/midi_io.h"

namespace Vleerhond
{
    const RandomParam tanzbar_cy_params[] = {
        {TB_CY_DECAY,  64, 127},
        {TB_CY_TONE ,   0, 127},
        {TB_CY_TUNE ,   0, 127}
    };
    const uint8_t nr_of_tanzbar_cy_params = sizeof(tanzbar_cy_params) / sizeof(*tanzbar_cy_params);

    class TanzbarCy : public Cymbal
    {
    protected:
        ModulationReceiver cy_tune;

    public:
        TanzbarCy(
            Modulators& modulators_ref,
            TimeStruct& time_ref) :
            Cymbal(modulators_ref, time_ref, MIDI_CHANNEL_TANZBAR),
            cy_tune(modulators_ref)
        {
            pitch = NOTE_TANZBAR_CY;
        }

        void randomize()
        {
            ofLogNotice("tanzbar_cy", "randomize()");
            Cymbal::randomize();
            Parameters::randomize_parameters(tanzbar_cy_params, nr_of_tanzbar_cy_params, MIDI_CC_CHANNEL_TANZBAR);

            // Modulators
            uint8_t range = Rand::randui8(32);
            this->cy_tune.randomize(range, 127 - range, .3);
        }

        bool play()
        {

            uint8_t value = 0;
            if (this->cy_tune.value(time, value))
            {
                MidiIO::send_cc(TB_CY_TUNE, value, MIDI_CC_CHANNEL_TANZBAR);
            }
            return Cymbal::play();
        }
    };
}
