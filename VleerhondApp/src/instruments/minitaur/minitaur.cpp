#include "instruments/minitaur/minitaur.h"

namespace Vleerhond {
Minitaur::Minitaur(
    HarmonyStruct& harmony, Modulators& modulators, TimeStruct& time)
    : InstrumentRack(time),
      bass_root(harmony, modulators, time),
      bass_funk(harmony, modulators, time),
      bass_long(harmony, modulators, time) {
    name = "Minitaur";
    instruments.push_back(&bass_root);
    instruments.push_back(&bass_funk);
    instruments.push_back(&bass_long);
}

void Minitaur::randomize() {
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
}  // namespace Vleerhond
