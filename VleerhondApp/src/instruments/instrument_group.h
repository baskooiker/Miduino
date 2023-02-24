#pragma once

#include "instrument_base.h"

namespace Vleerhond {
class InstrumentGroup : public TonalInstrumentBase {
   protected:
    std::vector<InstrumentBase*> instruments;

   public:
    InstrumentGroup(HarmonyStruct& harmony, TimeStruct& time);

    bool play();

    virtual void randomize();

    virtual std::vector<InstrumentBase*> getPtrs();
    virtual void autoRandomize();
    virtual void setChannel(std::shared_ptr<MidiChannel> channel);
    virtual std::shared_ptr<MidiChannel> getChannel();
};
}  // namespace Vleerhond