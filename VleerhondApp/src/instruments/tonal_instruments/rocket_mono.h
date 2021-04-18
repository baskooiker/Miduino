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
        TimeStruct& time) :
        Mono(harmony, modulators, time)
    {
    }
};
}