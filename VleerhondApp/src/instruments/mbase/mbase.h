#pragma once

#include "instruments/drums/kick.h"

namespace Vleerhond {

#define MBASE_TUNE 100
#define MBASE_PITCH 101
#define MBASE_DECAY 102
#define MBASE_HARMONICS 103
#define MBASE_PULSE 104
#define MBASE_NOISE 105
#define MBASE_ATTACK 106
#define MBASE_EQ 107
#define MBASE_COMPRESSION 113
#define MBASE_GATE_TIME 114
#define MBASE_METAL_NOISE 115
#define MBASE_VOLUME 117
#define MBASE_LFO_SYNC 120
#define MBASE_LFO_ONE_SHOT 123
#define MBASE_LFO_INTENSITY 121
#define MBASE_LFO_RATE 122

class MBase : public Kick {
   public:
    MBase(Modulators& modulators, TimeStruct& time);
};
}  // namespace Vleerhond