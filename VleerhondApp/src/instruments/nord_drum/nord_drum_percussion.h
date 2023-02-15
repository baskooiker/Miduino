#pragma once

#include "instrument_base.h"
#include "patterns/modulators.h"

namespace Vleerhond {
class NordDrumPercussion : public InstrumentBase {
    Modulators& modulators;

    std::vector<uint16_t> densities;
    std::vector<uint8_t> notes;
    CvPatternAB density_pattern;
    CvPatternAB velocity_pattern;
    uint8_t max_velocity;
    uint8_t min_velocity;

   public:
    NordDrumPercussion(Modulators& modulators, TimeStruct& time);

    bool play();
    void setDensity(uint8_t index, uint8_t value);
    void randomize();
    uint8_t getVelocity();
};
}  // namespace Vleerhond