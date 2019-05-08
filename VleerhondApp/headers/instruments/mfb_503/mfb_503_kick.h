#pragma once

#include "defs.h"
#include "parameters.h"
#include "kick.h"

namespace Vleerhond
{
    const RandomParam mfb_503_kick_params[] = {
        {BD_TUNE  ,  16, 64 },
        {BD_PITCH ,  16, 64 },
        {BD_DRIVE ,   0, 127},
        {BD_ATTACK,   0, 127},
    };
    const uint8_t nr_mfb_503_kick_params = sizeof(mfb_503_kick_params) / sizeof(RandomParam);

    class Mfb503Kick : public Kick
    {
    protected:
        HarmonyStruct& harmony;
        uint8_t volume;

    public:
        Mfb503Kick(
            Modulators& modulators_ref,
            HarmonyStruct& harmony_ref,
            TimeStruct& time_ref) :
            Kick(modulators_ref, time_ref),
            harmony(harmony_ref)
        {
            storage.set_channel(MIDI_CHANNEL_503);
            pitch = NOTE_503_BD;
        }

        virtual void randomize()
        {
            ofLogNotice("mfb_503_kick", "randomize()");
            Kick::randomize();
            Parameters::randomize_parameters(mfb_503_kick_params, nr_mfb_503_kick_params, MIDI_CHANNEL_503);
        }

        virtual void play()
        {
            pitch = Utils::clip_pitch(harmony.get_chord_step(time), NOTE_503_BD_MIN, NOTE_503_BD_MAX);
            Kick::play();
        }

        virtual void set_volume(const uint8_t value)
        {
            this->volume = value;
            MidiIO::send_cc(BD_LEVEL, value, MIDI_CHANNEL_503);
        }

        virtual uint8_t get_velocity()
        {
            //printf("Mfb503Kick::get_velocity()\n");

            bool quarter_hit = Utils::interval_hit(TimeDivision::Quarter, time);
            uint16_t velocity = quarter_hit ? 127 : 96;
            return (uint8_t)(((uint16_t)volume * velocity) / 127);
        }
    };
}