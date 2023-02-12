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

    bool play()
    { 
        auto it = std::max_element(std::begin(densities), std::end(densities));
        uint8_t max_density = *it;
        uint16_t v = density_pattern.value(time);

        if (v < max_density)
        {
            uint16_t total = 0;
            for (uint8_t i = 0; i < 4; i++)
            {
                total += densities[i];
                if (v < total)
                {
                    this->midi_channel->noteOn(
                        NoteStruct(notes[i], getVelocity()),
                        time.getShuffleDelay(this->timing)
                    );
                    return true;
                }
            }
        }

        return false;
    }

    void setDensity(uint8_t index, uint8_t value)
    {
        densities[index % 4] = value;
    }

    void randomize()
    {
        density_pattern.randomize();
    }
};
}