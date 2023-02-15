#pragma once

#include "core/defs.h"

namespace Vleerhond {
class MicroTimingStruct {
   public:
    int8_t shuffle_off;
    uint8_t delay;

    void randomize();
};
}  // namespace Vleerhond
