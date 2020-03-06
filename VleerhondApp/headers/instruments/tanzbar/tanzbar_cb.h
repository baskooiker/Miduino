#pragma once

#include "core/defs.h"
#include "instrument_base.h"
#include "patterns/modulators.h"
#include "patterns/gate_patterns.h"
#include "percussion.h"
#include "midi/midi_io.h"

namespace Vleerhond
{
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
            pitch = NOTE_TANZBAR_CB;

            this->params.push_back(CcParam(TB_CB_Tune, 0, 127));
            this->params.push_back(CcParam(TB_CB_Decay, 0, 64));
        }

        void randomize()
        {
            ofLogNotice("tanzbar_perc", "randomize()");
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
                getChannel()->sendCC(TB_CB_Tune, value);
            }
            return Percussion::play();
        }
    };
}