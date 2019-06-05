#pragma once

#include "parameters.h"
#include "drums/kick.h"

namespace Vleerhond
{
    const RandomParam tanzbar_low_params[] = {
        {TB_BD1_ATTACK   ,  0,  64},
        //{TB_BD1_DECAY    , 32,  96},
        {TB_BD1_PITCH    , 80, 112},
        {TB_BD1_TUNE     , 32,  96},
        {TB_BD1_NOISE    ,  0,  64},
        {TB_BD1_FILTER   ,  0,  64},
        {TB_BD1_DIST     ,  0, 127},
        {TB_BD1_TRIGGER  ,  0,  64},

        //{TB_BD2_DECAY    , 32,  96},
        {TB_BD2_TUNE     , 32,  96},
        {TB_BD2_TONE     , 32,  96}
    };
    const uint8_t nr_tanzbar_low_params = sizeof(tanzbar_low_params) / sizeof(RandomParam);

    class TanzbarLo : public Kick
    {
    protected:
        MicroTimingStruct bd_timing;
        ModulationReceiver bd2_pitch_mod;

    public:
        GatePatternAB bd_pattern;

        TanzbarLo(
            Modulators& modulators,
            TimeStruct& time) :
            Kick(modulators, time),
            bd2_pitch_mod(modulators)
        {
            storage.set_channel(MIDI_CHANNEL_TANZBAR);
            pitch = NOTE_TANZBAR_BD1;
        }

        void randomize()
        {
            ofLogNotice("tanzbar_lo", "randomize()");
            Kick::randomize();
            Parameters::randomize_parameters(tanzbar_low_params, nr_tanzbar_low_params, MIDI_CC_CHANNEL_TANZBAR);

            uint8_t range = Rand::randui8(64);
            bd2_pitch_mod.randomize(range, 32, .3);
        }

        void play()
        {
            if (kill)
            {
                return;
            }

            bool quarter_hit = Utils::interval_hit(TimeDivision::Quarter, time);
            if (quarter_hit)
            {
                this->pitch = NOTE_TANZBAR_BD1;
            }
            else
            {
                this->pitch = NOTE_TANZBAR_BD2;
                uint8_t value = 0;
                if (bd2_pitch_mod.value(time, value))
                {
                    MidiIO::send_cc(TB_BD2_TUNE, value, MIDI_CC_CHANNEL_TANZBAR);
                }
            }
            Kick::play();

            //if (quarter_hit && !this->kill)
            //{
            //    this->storage.note_on(
            //        NoteStruct(NOTE_TANZBAR_BD2, 127),
            //        time.get_shuffle_delay(this->bd_timing)
            //    );
            //}
        }
    };
}