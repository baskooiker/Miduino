#include "instruments/vermona/vermona.h"

namespace Vleerhond
{
    Vermona::Vermona(HarmonyStruct & harmony, TimeStruct & time)
        : InstrumentRack(time)
        , poly(harmony, time)
        , fugue(harmony, time)
    {
        instruments.push_back(&poly);
        instruments.push_back(&fugue);
    }

    void Vermona::select(const uint8_t index)
    {
        uint8_t n_index = index % instruments.size();
        switch (n_index)
        {
        case 0:
            getChannel()->sendCC(CC_VERMONA_MODE, VERMONA_POLYPHONIC);
            getChannel()->sendCC(CC_VERMONA_SYNTH_1_CH, 7);
            getChannel()->sendCC(CC_VERMONA_SYNTH_2_CH, 7);
            getChannel()->sendCC(CC_VERMONA_SYNTH_3_CH, 7);
            getChannel()->sendCC(CC_VERMONA_SYNTH_4_CH, 7);
            break;
        case 1:
            getChannel()->sendCC(CC_VERMONA_MODE, VERMONA_UNISONO);
            getChannel()->sendCC(CC_VERMONA_SYNTH_1_CH, 7);
            getChannel()->sendCC(CC_VERMONA_SYNTH_2_CH, 8);
            getChannel()->sendCC(CC_VERMONA_SYNTH_3_CH, 9);
            getChannel()->sendCC(CC_VERMONA_SYNTH_4_CH, 10);
            break;
        }

        this->selection = n_index;
    }

}