#pragma once

#include "instrument_base.h"

namespace Vleerhond
{
class InstrumentGroup : public TonalInstrumentBase
{
protected:
    std::vector<InstrumentBase*> instruments;

public:
    InstrumentGroup(HarmonyStruct& harmony, TimeStruct& time):
        TonalInstrumentBase(harmony, time, false)
    {
    }

    bool play()
    {
        bool rv = false;
        for (InstrumentBase* instrument : instruments)
        {
            rv |= instrument->play();
        }
        return rv;
    }

    void randomize()
    {
        for (InstrumentBase* instrument : instruments)
        {
            instrument->randomize();
        }
    }
};
}