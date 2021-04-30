#pragma once

#include "instruments/instrument_rack.h"

#include "instruments/minitaur/moog_bass.h"

namespace Vleerhond
{

    class Minitaur : public InstrumentRack
    {
    public:
        MoogBass bass_root;
        MoogBass bass_funk;
        LongBass bass_long;

        Minitaur(Modulators& modulators, HarmonyStruct& harmony, TimeStruct& time);

        void randomize();
    };
}
