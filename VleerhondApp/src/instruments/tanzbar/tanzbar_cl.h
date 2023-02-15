#pragma once

#include "instruments/drums/percussion.h"
#include "patterns/modulators.h"

namespace Vleerhond {
class TanzbarCl : public Percussion {
   protected:
    ModulationReceiver cl_pitch;

   public:
    TanzbarCl(Modulators& modulators_ref, TimeStruct& time_ref);

    void randomize();

    bool play();
};
}  // namespace Vleerhond