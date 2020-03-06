#pragma once

#include "instruments/drums/hats.h"
#include "midi/midi_io.h"

namespace Vleerhond
{
    class TanzbarHats : public Hats
    {
    protected:
        ModulationReceiver tune_mod;
        ModulationReceiver oh_dec;

    public:
        GatePatternAB hh_pattern;
        GatePatternAB oh_pattern;

        TanzbarHats(
            Modulators& modulators,
            TimeStruct& time) :
            Hats(modulators, time),
            tune_mod(modulators),
            oh_dec(modulators)
        {
            pitch_open = NOTE_TANZBAR_OH;
            pitch_closed = NOTE_TANZBAR_HH;

            settings.p_euclid = 0;
            settings.p_drop = 0;

            this->params.push_back(CcParam(TB_HH_TUNE, 0, 127));
        }

        void randomize()
        {
            ofLogNotice("tanzbar_hi", "randomize()");
            Hats::randomize();

            // Modulators
            {
                uint8_t range = Rand::randui8(16, 96);
                uint8_t off = Rand::randui8(127 - range);
                this->tune_mod.randomize(range, off, .33);
            }

            {
                uint8_t range = Rand::randui8(16, 64);
                uint8_t off = Rand::randui8(96 - range);
                this->oh_dec.randomize(range, off, .33);
            }

        }

        bool play()
        {
            if (this->isKilled())
            {
                return false;
            }

            uint8_t value = 0;
            if (tune_mod.value(time, value))
            {
                getChannel()->sendCC(TB_HH_TUNE, value);
            }
            return Hats::play();
        }
    };
}