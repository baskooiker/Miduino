#pragma once

#include "bass.h"

namespace Vleerhond
{
    class AcidBass : public Bass
    {
    public:
        AcidBass(
            HarmonyStruct& harmony,
            TimeStruct& time) :
            Bass(harmony, time)
        {
            midi_channel.set_channel(MIDI_CHANNEL_BASS, 24);
        }

        virtual void randomize()
        {
            Bass::randomize();
            note_range_value = Rand::randui8(32, 64);
        }
    };
}
