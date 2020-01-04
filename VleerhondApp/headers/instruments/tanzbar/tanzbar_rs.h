#pragma once

#include "core/defs.h"
#include "instruments/instrument_base.h"
#include "patterns/modulators.h"
#include "patterns/gate_patterns.h"
#include "instruments/parameters.h"
#include "instruments/drums/percussion.h"

namespace Vleerhond
{
    class TanzbarRs : public Percussion
    {
    protected:
        ModulationReceiver rs_tune;

    public:
        TanzbarRs(
            Modulators& modulators_ref,
            TimeStruct& time_ref) :
            Percussion(modulators_ref, time_ref, MIDI_CHANNEL_TANZBAR),
            rs_tune(modulators_ref)
        {
            this->params.push_back({ TB_RS_TUNE, 0, 127, MIDI_CHANNEL_TANZBAR });
        }

        void randomize()
        {
            Percussion::randomize();
            randomize_parameters();

            // Modulators
            uint8_t range = Rand::randui8(128);
            this->rs_tune.randomize(range, 127 - range, .3);
        }

        bool play()
        {
            uint8_t value = 0;
            if (this->rs_tune.value(time, value))
            {
                MidiIO::send_cc(TB_RS_TUNE, value, MIDI_CC_CHANNEL_TANZBAR);
            }
            return Percussion::play();
        }
    };
}