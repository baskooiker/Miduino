#pragma once

#include "mono.h"
#include "patterns/modulators.h"

namespace Vleerhond
{
class RocketMono : public Mono
{
public:
    RocketMono(
        HarmonyStruct& harmony,
        Modulators& modulators,
        TimeStruct& time,
        const uint8_t midi_channel) :
        Mono(harmony, modulators, time, MIDI_CHANNEL_ROCKET)
    {
    }
};
}