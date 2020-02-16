#pragma once

#include "parameters.h"
#include "drums/kick.h"
#include "midi/midi_io.h"

namespace Vleerhond
{
    const RandomParam tanzbar_low_params[] = {
        //{TB_BD1_ATTACK   ,  0,  64},
        //{TB_BD1_DECAY    , 32,  96},
        //{TB_BD1_PITCH    , 80, 112},
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
        TanzbarLo(
            Modulators& modulators,
            TimeStruct& time) :
            Kick(modulators, time, MIDI_CHANNEL_TANZBAR),
            bd2_pitch_mod(modulators)
        {
            this->pitch = NOTE_TANZBAR_BD1;
        }

        void randomize()
        {
            ofLogNotice("tanzbar_lo", "randomize()");
            Kick::randomize();

            Parameters::randomize_parameters(tanzbar_low_params, nr_tanzbar_low_params, MIDI_CC_CHANNEL_TANZBAR);

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
                    MidiIO::send_cc(TB_BD2_TUNE, value, MIDI_CC_CHANNEL_TANZBAR);
                }
                this->midi_channel.note_on(
                    NoteStruct(NOTE_TANZBAR_BD2, get_velocity()),
                    time.get_shuffle_delay(this->timing)
                );
                rv = true;
            }
            return rv;
        }
    };
}