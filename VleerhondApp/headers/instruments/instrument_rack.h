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
        const uint8_t midi_channel);

public:
    bool play();
    void randomize();
    virtual std::vector<InstrumentBase*> get_ptrs();
    virtual void auto_randomize();
    void select(const uint8_t index);

    virtual void setVariableDensity(const uint8_t variable_density);
    virtual uint8_t getVariableDensity() const;
    virtual void setVariablePitch(const uint8_t variable_pitch);
    virtual uint8_t getVariablePitch() const;
    virtual void setVariablePitchOffset(const uint8_t variable_pitch_offset);
    virtual uint8_t getVariablePitchOffset() const;
    virtual void kill(const bool kill);
    virtual void setChannel(std::shared_ptr<MidiChannel> channel);
    virtual std::shared_ptr<MidiChannel> getChannel();
    virtual bool getPedal() override;
private:
    InstrumentBase* getInstr();
    InstrumentBase* getInstr() const;
};

}