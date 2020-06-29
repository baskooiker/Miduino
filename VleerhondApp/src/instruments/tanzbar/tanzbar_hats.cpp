#include "instruments/tanzbar/tanzbar_hats.h"

#include "utils/rand.h"

namespace Vleerhond
{
    TanzbarHats::TanzbarHats(Modulators & modulators, TimeStruct & time) :
        Hats(modulators, time),
        tune_mod(modulators)
    {
        pitch_open = NOTE_TANZBAR_OH;
        pitch_closed = NOTE_TANZBAR_HH;

        settings.p_euclid = 0;
        settings.p_drop = 0;
        settings.p_3_4 = 0;

        this->params.push_back(CcParam(TB_HH_TUNE, 0, 127));
    }
    void TanzbarHats::randomize()
    {
        ofLogNotice("tanzbar_hi", "randomize()");
        Hats::randomize();

        // Modulator
        {
            uint8_t range = Rand::randui8(16, 96);
            uint8_t off = Rand::randui8(127 - range);
            this->tune_mod.randomize(range, off, .33);
        }


    }
    bool TanzbarHats::play()
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
}
