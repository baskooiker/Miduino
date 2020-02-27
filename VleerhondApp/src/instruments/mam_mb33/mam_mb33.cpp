#include "instruments/mam_mb33/mam_mb33.h"

namespace Vleerhond
{
    MamMb33::MamMb33(HarmonyStruct & harmony, Modulators & modulators, TimeStruct & time)
        : InstrumentRack(time, 0)
        , drop_bass(harmony, time)
        , const_bass(harmony, time)
        , acid_bass(harmony, time)
    {
        instruments.push_back(&drop_bass);
        instruments.push_back(&const_bass);
        instruments.push_back(&acid_bass);
    }

    void MamMb33::randomize()
    {
        // drop bass
        {
            drop_bass.total_randomize();
            drop_bass.randomize_drop();
        }

        // const bass
        {
            const_bass.total_randomize();
            const_bass.note_range_value = 0;
        }

        // acid bass
        {
            acid_bass.total_randomize();
            acid_bass.note_range_value = 96;
            acid_bass.variable_octave = 96;
        }
    }
}
