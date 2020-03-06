#pragma once

#include "core/defs.h"
#include "cymbal.h"

namespace Vleerhond
{
    const RandomParam mfb_503_cymbal_params[] = {
        {MFB_503_CY_MIX  , 100, 127},
        {MFB_503_CY_DECAY, 120, 127},
        {MFB_503_CY_TUNE , 100, 127}
    };
    const uint8_t nr_mfb_503_cymbal_params = sizeof(mfb_503_cymbal_params) / sizeof(RandomParam);

    class Mfb503Cymbal : public Cymbal
    {
    protected:
        uint8_t volume;

    public:
        Mfb503Cymbal(
            Modulators& modulators_ref,
            TimeStruct& time_ref) :
            Cymbal(modulators_ref, time_ref, MIDI_CHANNEL_503)
        {

            // TODO: randomize 
            pitch = NOTE_503_CY_MAX;

            this->randomize();
        }

        void randomize()
        {
            ofLogNotice("mfb_503_cymbal", "randomize()");
            Cymbal::randomize();
        }

        void set_volume(const uint8_t value)
        {
            this->volume = value;
            getChannel()->sendCC(MFB_503_CY_LEVEL, value);
        }

        virtual uint8_t get_velocity()
        {
            return (uint8_t)(((uint16_t)volume * (uint16_t)Cymbal::get_velocity()) / 127);
        }
    };
}