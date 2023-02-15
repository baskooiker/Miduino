#pragma once

#include "instruments/drums/hats.h"
#include "instruments/instrument_rack.h"
#include "midi/midi_io.h"
#include "patterns/modulators.h"

namespace Vleerhond {
class AbstractTanzbarHats : public Hats {
   protected:
    ModulationReceiver tune_mod;

   public:
    AbstractTanzbarHats(Modulators& modulators, TimeStruct& time);
    bool play();
};

class TanzbarRegularHats : public AbstractTanzbarHats {
   public:
    TanzbarRegularHats(Modulators& modulators, TimeStruct& time);
    void randomize();
};

class TanzbarAlternativeHats : public AbstractTanzbarHats {
   public:
    TanzbarAlternativeHats(Modulators& modulators, TimeStruct& time);
    void randomize();
};

class TanzbarHats : public InstrumentRack {
   public:
    TanzbarRegularHats regular_hats;
    TanzbarAlternativeHats alternative_hats;

    TanzbarHats(Modulators& modulators, TimeStruct& time)
        : InstrumentRack(time),
          regular_hats(modulators, time),
          alternative_hats(modulators, time) {
        instruments.push_back(&regular_hats);
        instruments.push_back(&alternative_hats);
    }
};

}  // namespace Vleerhond