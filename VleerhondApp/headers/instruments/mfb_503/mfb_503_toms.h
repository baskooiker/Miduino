#pragma once

#include "toms.h"

namespace Vleerhond
{
    const RandomParam mfb_503_toms_params[] = {
        {MFB_503_LT_TUNE ,  16,  64},
        {MFB_503_LT_DECAY,  32,  96},
        {MFB_503_LT_PITCH,  32,  96},

        {MFB_503_MT_TUNE ,  32,  80},
        {MFB_503_MT_DECAY,  32,  96},
        {MFB_503_MT_PITCH,  32,  96},

        {MFB_503_MT_TUNE ,  48,  96},
        {MFB_503_MT_DECAY,  32,  96},
        {MFB_503_MT_PITCH,  32,  96},
    };
    const uint8_t nr_mfb_503_toms_params = sizeof(mfb_503_toms_params) / sizeof(RandomParam);

    class Mfb503Toms : public Toms
    {
    protected:
        uint8_t volume;
        HarmonyStruct& harmony;
        std::vector<uint8_t> penta_notes;

    public:
        Mfb503Toms(
            Modulators& modulators_ref,
            HarmonyStruct& harmony_ref,
            TimeStruct& time_ref) :
            Toms(modulators_ref, time_ref, MIDI_CHANNEL_503),
            harmony(harmony_ref)
        {
            this->randomize();
        }

        virtual void randomize()
        {
            ofLogNotice("mfb_503_toms", "randomize()");
            Toms::randomize();

            uint8_t nr_notes = Rand::randui8(2, 4);
            penta_notes.clear();
            uint8_t notes[] = {0, 1, 2, 3, 4};
            Utils::randomizeOrder(notes, 5);
            for (int i = 0; i < nr_notes; i++)
            {
                penta_notes.push_back(notes[i]);
            }
        }

        virtual void set_volume(const uint8_t value)
        {
            this->volume = value;
            getChannel()->sendCC(MFB_503_LT_LEVEL, value);
            getChannel()->sendCC(MFB_503_MT_LEVEL, value);
            getChannel()->sendCC(MFB_503_HT_LEVEL, value);
        }

        virtual uint8_t getVelocity()
        {
            return (uint8_t)(((uint16_t)volume * (uint16_t)Toms::getVelocity()) / 127);
        }

        virtual uint8_t getPitch(const TimeStruct& time)
        {
            uint8_t prob = tom_pattern.value(time);
            uint8_t penta = penta_notes.at(prob % penta_notes.size());
            uint8_t penta_pitch = harmony.scale.getPenta(penta);
            return Utils::clipPitch(penta_pitch, NOTE_503_TOM_MIN, NOTE_503_TOM_MAX);
        }
    };
}