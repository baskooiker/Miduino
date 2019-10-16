#pragma once

#include "bass.h"

namespace Vleerhond
{
    class AcidBass : public Bass
    {
    private:
        uint8_t accent_amount;

    public:
        AcidBass(
            HarmonyStruct& harmony,
            TimeStruct& time) :
            Bass(harmony, time)
        {
            midi_channel.set_channel(MIDI_CHANNEL_BASS, 24);
            MidiIO::send_cc(VERMONA_SYNTH_1_MOD_WHEEL, 127, VERMONA_CONTROL_CHANNEL);

            settings.p_euclid_16 = 16;
            settings.p_euclid_8 = 0;
            settings.p_interval = 0;
            settings.p_diddles = 32;

            settings.euclid_16.p_5 = 0;
            settings.euclid_16.p_6 = 0;
            settings.euclid_16.p_7 = 0;
            settings.euclid_16.p_9 = 16;
            settings.euclid_16.p_11 = 16;

            settings.diddles.p_min = .6;
            settings.diddles.p_max = .9;

        }

        virtual void randomize()
        {
            Bass::randomize();
            note_range_value = Rand::randui8(16, 56);
            accent_amount = Rand::randui8(64, 96);
        }

        virtual bool play()
        {
            if (kill)
                return false;

            if (this->get_bass_hit(this->density, time))
            {
                uint8_t value = 0;
                if (this->accents.gate(this->time))
                {
                    value = accent_amount;
                }
                MidiIO::send_cc(MODULATION_WHEEL_CC, value, MIDI_CHANNEL_BASS);
            }
            return Bass::play();
        }
    };
}
