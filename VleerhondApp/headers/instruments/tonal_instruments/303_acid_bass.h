#pragma once

#include "bass.h"

namespace Vleerhond
{
    class TB303Bass 
        : public Bass
    {
    public:
        TB303Bass(
            HarmonyStruct& harmony,
            TimeStruct& time) :
            Bass(harmony, time)
        {
            settings.p_euclid_16 = 16;
            settings.p_euclid_8 = 32;
            settings.p_interval = 0;
            settings.p_diddles = 32;

            settings.euclid_16.p_5 = 0;
            settings.euclid_16.p_6 = 0;
            settings.euclid_16.p_7 = 0;
            settings.euclid_16.p_9 = 16;
            settings.euclid_16.p_11 = 16;

            settings.diddles.p_min = .6;
            settings.diddles.p_max = .9;

            follow_harmony = false;
        }

        void randomize()
        {
            Bass::randomize();
            note_range_value = Rand::randui8(16, 56);
            variable_octave = Rand::randui8(64);
        }

        uint8_t get_velocity()
        {
            if (this->accents.gate(this->time))
            {
                return 127;
            }
            return 64;
        }
    };
}
