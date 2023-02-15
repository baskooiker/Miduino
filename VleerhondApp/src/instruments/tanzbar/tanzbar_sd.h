#pragma once

#include "instruments/drums/snare.h"

namespace Vleerhond {
class TanzbarMid : public Snare {
   public:
    TanzbarMid(Modulators& modulators, TimeStruct& time);
};
}  // namespace Vleerhond