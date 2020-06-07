#include "instruments/tanzbar/tanzbar_lo.h"

namespace Vleerhond
{
    TanzbarLo::TanzbarLo(Modulators & modulators, TimeStruct & time) :
        Kick(modulators, time),
        bd2_pitch_mod(modulators)
    {
        this->pitch = NOTE_TANZBAR_BD1;

        this->params.push_back(CcParam(TB_BD1_TUNE, 32, 48));
        this->params.push_back(CcParam(TB_BD1_NOISE, 0, 32));
        this->params.push_back(CcParam(TB_BD1_FILTER, 0, 32));
        this->params.push_back(CcParam(TB_BD1_DIST, 64, 127));
        this->params.push_back(CcParam(TB_BD1_TRIGGER, 0, 127));
        this->params.push_back(CcParam(TB_BD2_TUNE, 32, 96));
        this->params.push_back(CcParam(TB_BD2_TONE, 32, 96));
    }
    void TanzbarLo::randomize()
    {
        ofLogNotice("tanzbar_lo", "randomize()");
        Kick::randomize();

        uint8_t range = Rand::randui8(96);
        bd2_pitch_mod.randomize(range, 32, .3);
    }

    void TanzbarLo::randomize_alternative()
    {
        // clear pattern
        bd_pattern.setAll(0);

        uint8_t length = 8;
        uint8_t steps = 3;
        switch (Rand::distribution(16, 16))
        {
        case 0:
            length = 8;
            steps = 3;
            break;
        case 1:
            length = 16;
            steps = Rand::distribution(16, 16) == 0 ? 5 : 6;
            break;
        }

        bd_pattern.setEuclid(length, steps);
        bd_pattern.abPattern.setConst();
        bd_pattern.length = length;
    }

    bool TanzbarLo::play()
    {
        if (isKilled())
        {
            return false;
        }

        bool rv = Kick::play();

        if (Utils::intervalHit(TimeDivision::Quarter, time))
        {
            uint8_t value = 0;
            if (bd2_pitch_mod.value(time, value))
            {
                getChannel()->sendCC(TB_BD2_TUNE, value);
            }
            this->midi_channel->noteOn(
                NoteStruct(NOTE_TANZBAR_BD2, getVelocity()),
                time.getShuffleDelay(this->timing)
            );
            rv = true;
        }
        return rv;
    }
}
