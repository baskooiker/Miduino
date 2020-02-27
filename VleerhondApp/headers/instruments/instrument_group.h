#pragma once

#include "instrument_base.h"

namespace Vleerhond
{
class InstrumentGroup : public TonalInstrumentBase
{
protected:
    std::vector<InstrumentBase*> instruments;

public:
    InstrumentGroup(HarmonyStruct& harmony, TimeStruct& time);

    bool play();

    void randomize();

    virtual std::vector<InstrumentBase*> get_ptrs();

    virtual void auto_randomize();

    virtual void setChannel(std::shared_ptr<MidiChannel> channel);
    virtual std::shared_ptr<MidiChannel> getChannel();
};
}