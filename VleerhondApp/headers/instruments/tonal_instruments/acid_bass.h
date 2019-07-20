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
        }

        virtual void randomize()
        {
            Bass::randomize();
            note_range_value = Rand::randui8(16, 56);
            accent_amount = Rand::randui8(64, 96);
        }

        virtual bool play()
        {
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
