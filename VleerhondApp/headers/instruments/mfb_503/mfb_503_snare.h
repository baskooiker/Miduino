#pragma once

#include "core/defs.h"
#include "snare.h"

namespace Vleerhond
{
    const RandomParam mfb_503_snare_params[] = {
        {BD_TUNE  ,  16, 64 },
        {BD_PITCH ,  16, 64 },
        {BD_DRIVE ,   0, 127},
        {BD_ATTACK,   0, 127},
    };
    const uint8_t nr_mfb_503_snare_params = sizeof(mfb_503_snare_params) / sizeof(RandomParam);

    class Mfb503Snare : public Snare
    {
    protected:
        uint8_t volume;

    public:
        Mfb503Snare(
            Modulators& modulators_ref,
            TimeStruct& time_ref) :
            Snare(modulators_ref, time_ref, MIDI_CHANNEL_503)
        {
            pitch = NOTE_503_SD_MIN;
        }

        virtual void randomize()
        {
            ofLogNotice("mfb_503_snare", "randomize()");
            Snare::randomize();
        }

        virtual void set_volume(const uint8_t value)
        {
            this->volume = value;
            getChannel()->sendCC(SD_LEVEL, value);
        }

        virtual uint8_t getVelocity()
        {
            return (uint8_t)(((uint16_t)volume * (uint16_t)Snare::getVelocity()) / 127);
        }
    };
}