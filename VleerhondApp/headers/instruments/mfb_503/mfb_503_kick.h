#pragma once

#include "core/defs.h"
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
            Kick(modulators_ref, time_ref, MIDI_CHANNEL_503),
            harmony(harmony_ref)
        {
            pitch = NOTE_503_BD;
        }

        virtual void randomize()
        {
            ofLogNotice("mfb_503_kick", "randomize()");
            Kick::randomize();
        }

        virtual bool play()
        {
            pitch = Utils::clipPitch(harmony.getChordStep(time).root, NOTE_503_BD_MIN, NOTE_503_BD_MAX);
            return Kick::play();
        }

        virtual void set_volume(const uint8_t value)
        {
            this->volume = value;
            getChannel()->sendCC(BD_LEVEL, value);
        }

        virtual uint8_t getVelocity()
        {
            //printf("Mfb503Kick::getVelocity()\n");

            bool quarter_hit = Utils::intervalHit(TimeDivision::Quarter, time);
            uint16_t velocity = quarter_hit ? 127 : 96;
            return (uint8_t)(((uint16_t)volume * velocity) / 127);
        }

        void set_decay(const uint8_t decay)
        {
            getChannel()->sendCC(BD_DECAY, decay);
        }
    };
}