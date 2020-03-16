#include "instruments/minitaur/minitaur.h"

namespace Vleerhond
{
    Minitaur::Minitaur(Modulators & modulators, HarmonyStruct & harmony, TimeStruct & time)
        : InstrumentRack(time)
        , bass_root(modulators, harmony, time)
        , bass_funk(modulators, harmony, time)
    {
        instruments.push_back(&bass_root);
        instruments.push_back(&bass_funk);
    }
    void Minitaur::randomize()
    {
        {
            bass_root.total_randomize();
            bass_root.note_range_value = 0;
        }

        {
            bass_funk.total_randomize();
            bass_funk.note_range_value = 127;
        }
    }
}