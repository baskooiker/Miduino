#include "nord_drum.h"

#include "instrument_group.h"

namespace Vleerhond
{
    NordDrum::NordDrum(Modulators& modulators, TimeStruct& time) :
        InstrumentBase(time),
        modulators(modulators)
    {
        name = "nord_drum";
        notes = {36, 37, 39, 40};
        densities = {0, 0, 0, 0};
        ofLogNotice("Hey", "Hoi");
    }

    bool NordDrum::play()
    { 
        ofLogNotice("NordDrum", "play");
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

    void NordDrum::setDensity(uint8_t index, uint8_t value)
    {
        densities[index % 4] = value;
    }

    void NordDrum::randomize()
    {
        density_pattern.randomize();
    }
}