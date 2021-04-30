#include "instruments/minitaur/minitaur.h"

namespace Vleerhond
{
    Minitaur::Minitaur(Modulators & modulators, HarmonyStruct & harmony, TimeStruct & time)
        : InstrumentRack(time)
        , bass_root(modulators, harmony, time)
        , bass_funk(modulators, harmony, time)
        , bass_long(modulators, harmony, time)
    {
        name = "Minitaur";

        instruments.push_back(&bass_root);
        instruments.push_back(&bass_funk);
        instruments.push_back(&bass_long);
    }

    void Minitaur::randomize()
    {
        {
            bass_root.totalRandomize();
            bass_root.note_range_value = 0;
        }

        {
            bass_funk.totalRandomize();
            bass_funk.note_range_value = 127;
        }

        bass_long.randomize();
    }
}
