#pragma once

#include "defs.hpp"
#include "parameters.hpp"
#include "snare.hpp"

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
            Snare(modulators_ref, time_ref)
        {
            midi_channel.set_channel(MIDI_CHANNEL_503);
            pitch = NOTE_503_SD_MIN;
        }

        virtual void randomize()
        {
            ofLogNotice("mfb_503_snare", "randomize()");
            Snare::randomize();
            Parameters::randomize_parameters(mfb_503_snare_params, nr_mfb_503_snare_params, MIDI_CHANNEL_503);
        }

        virtual void set_volume(const uint8_t value)
        {
            this->volume = value;
            MidiIO::send_cc(SD_LEVEL, value, MIDI_CHANNEL_503);
        }

        virtual uint8_t get_velocity()
        {
            return (uint8_t)(((uint16_t)volume * (uint16_t)Snare::get_velocity()) / 127);
        }
    };
}