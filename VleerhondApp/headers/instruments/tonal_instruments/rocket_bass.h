#pragma once

#include "bass.h"

namespace Vleerhond
{
    class RocketBass : public Bass
    {
    public:
        RocketBass(
            HarmonyStruct& harmony,
            TimeStruct& time) :
            Bass(harmony, time)
        {
            storage.set_channel(MIDI_CHANNEL_ROCKET);
        }
    };
}
