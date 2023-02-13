#pragma once

#include "instrument_base.h"
#include "patterns/modulators.h"

namespace Vleerhond
{
class NordDrum : public InstrumentBase
{
    Modulators& modulators;

    std::vector<uint8_t> densities;
    std::vector<uint8_t> notes;
    CvPatternAB density_pattern;

public:
    NordDrum(Modulators& modulators, TimeStruct& time);

    bool play();
    void setDensity(uint8_t index, uint8_t value);
    void randomize();
};
}