#pragma once

#include "bass.h"

namespace Vleerhond
{
    class RocketBass : public Bass
    {
    protected:
        uint16_t volume;

    public:
        RocketBass(
            HarmonyStruct& harmony,
            TimeStruct& time) :
            Bass(harmony, time)
        {
            storage.set_channel(MIDI_CHANNEL_MINITAUR);
            volume = 100;
        }

        void set_volume(const uint8_t value)
        {
            this->volume = (uint16_t)value;
        }

        virtual uint8_t get_velocity()
        {
            return (uint8_t)((uint16_t)Bass::get_velocity() *  this->volume / 127);
        }
    };
}
