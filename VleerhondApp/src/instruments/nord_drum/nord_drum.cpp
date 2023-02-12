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
        notes = {0, 0, 0, 0};
    }
}