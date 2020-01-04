#pragma once

#include "instrument_base.h"

namespace Vleerhond
{

class InstrumentRack : public InstrumentBase
{
protected:
    std::vector<InstrumentBase*> instruments;
    uint8_t selection = 0;

public:
    InstrumentRack(
        TimeStruct& time,
        const uint8_t midi_channel)
        : InstrumentBase(time, true, midi_channel)
    {
    }

public:
    bool play()
    {
        return instruments[selection % instruments.size()]->play();
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
        return { instruments[selection % instruments.size()] };
    }

    virtual void auto_randomize()
    {
        return instruments[selection % instruments.size()]->auto_randomize();
    }
};

}