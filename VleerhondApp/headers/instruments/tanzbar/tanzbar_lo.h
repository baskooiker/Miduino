#pragma once

#include "drums/kick.h"
#include "midi/midi_io.h"

namespace Vleerhond
{
    class TanzbarLo : public Kick
    {
    protected:
        MicroTimingStruct bd_timing;
        ModulationReceiver bd2_pitch_mod;

    public:
        TanzbarLo(
            Modulators& modulators,
            TimeStruct& time) :
            Kick(modulators, time),
            bd2_pitch_mod(modulators)
        {
            this->pitch = NOTE_TANZBAR_BD1;

            this->params.push_back(CcParam(TB_BD1_TUNE   , 32,  48));
            this->params.push_back(CcParam(TB_BD1_NOISE  ,  0,  32));
            this->params.push_back(CcParam(TB_BD1_FILTER ,  0,  32));
            this->params.push_back(CcParam(TB_BD1_DIST   , 64, 127));
            this->params.push_back(CcParam(TB_BD1_TRIGGER,  0, 127));
            this->params.push_back(CcParam(TB_BD2_TUNE   , 32,  96));
            this->params.push_back(CcParam(TB_BD2_TONE   , 32,  96));
        }

        void randomize()
        {
            ofLogNotice("tanzbar_lo", "randomize()");
            Kick::randomize();

            uint8_t range = Rand::randui8(96);
            bd2_pitch_mod.randomize(range, 32, .3);
        }

        void randomize_alternative()
        {
            // clear pattern
            bd_pattern.set_all(0);

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

            bd_pattern.set_euclid(length, steps);
            bd_pattern.abPattern.set_ab_pattern_const();
            bd_pattern.length = length;
        }

        bool play()
        {
            if (isKilled())
            {
                return false;
            }

            bool rv = Kick::play();
            if (Utils::interval_hit(TimeDivision::Quarter, time))
            {
                uint8_t value = 0;
                if (bd2_pitch_mod.value(time, value))
                {
                    getChannel()->sendCC(TB_BD2_TUNE, value);
                }
                this->midi_channel->note_on(
                    NoteStruct(NOTE_TANZBAR_BD2, get_velocity()),
                    time.get_shuffle_delay(this->timing)
                );
                rv = true;
            }
            return rv;
        }
    };
}