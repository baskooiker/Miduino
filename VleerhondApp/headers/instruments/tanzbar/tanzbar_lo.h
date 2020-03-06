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