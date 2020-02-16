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
        TonalInstrumentBase(harmony, time, false, 0)
    {
    }

    bool play()
    {
        if (this->_kill)
        {
            return false;
        }

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

    virtual std::vector<InstrumentBase*> get_ptrs()
    {
        return instruments;
    }

    virtual void auto_randomize()
    {
        for (InstrumentBase* instrument : instruments)
        {
            instrument->auto_randomize();
        }
    }
};
}