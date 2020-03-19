#pragma once

#include "hats.h"

namespace Vleerhond
{
    const RandomParam mfb_503_hats_params[] = {
        {HH_MIX   ,   0, 127},
        {OH_DECAY ,  32, 84 },
        {HH_DECAY ,   0, 64 },
    };
    const uint8_t nr_mfb_503_hats_params = sizeof(mfb_503_hats_params) / sizeof(RandomParam);

    class Mfb503Hats : public Hats
    {
    protected:
        uint8_t volume;

    public:
        Mfb503Hats(
            Modulators& modulators_ref,
            TimeStruct& time_ref) :
            Hats(modulators_ref, time_ref, MIDI_CHANNEL_503)
        {
            pitch_closed = NOTE_503_HH;
            pitch_open = NOTE_503_OH;

            getChannel()->sendCC(HH_LEVEL, 0);
        }

        void randomize()
        {
            ofLogNotice("mfb_503_hats", "randomize()");
            Hats::randomize();
        }

        void set_volume(const uint8_t value)
        {
            this->volume = value;
            getChannel()->sendCC(HH_LEVEL, value);
        }

        uint8_t getVelocity()
        {
            uint16_t velocity = 63;
            if ((time.tick / TICKS_PER_STEP) % 4 == 2)
            {
                velocity = 127;
            }
            else
            {
                velocity = Utils::rerange(this->hat_velocity.value(time), 64, 63);
            }
            return (uint8_t)(((uint16_t)volume * velocity) / 127);
        }
    };
}

