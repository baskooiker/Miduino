#include "instrument_rack.h"

namespace Vleerhond
{
    InstrumentRack::InstrumentRack(TimeStruct & time, const uint8_t midi_channel)
        : InstrumentBase(time, true, midi_channel)
    {
    }

    bool InstrumentRack::play()
    {
        return instruments[selection % instruments.size()]->play();
    }
    inline void InstrumentRack::randomize()
    {
        for (InstrumentBase* instrument : instruments)
        {
            instrument->randomize();
        }
    }
    inline std::vector<InstrumentBase*> InstrumentRack::get_ptrs()
    {
        return { instruments[selection % instruments.size()] };
    }
    inline void InstrumentRack::auto_randomize()
    {
        return instruments[selection % instruments.size()]->auto_randomize();
    }
    void InstrumentRack::select(const uint8_t index)
    {
        instruments[selection % instruments.size()]->stop_notes();
        instruments[selection % instruments.size()]->kill(false);
        this->selection = index;
    }
    void InstrumentRack::setVariableDensity(const uint8_t variable_density)
    {
        return getInstr()->setVariableDensity(variable_density);
    }
    uint8_t InstrumentRack::getVariableDensity() const
    {
        return getInstr()->getVariableDensity();
    }
    void InstrumentRack::setVariablePitch(const uint8_t variable_pitch)
    {
        return getInstr()->setVariablePitch(variable_pitch);
    }
    uint8_t InstrumentRack::getVariablePitch() const
    {
        return getInstr()->getVariablePitch();
    }
    void InstrumentRack::setVariablePitchOffset(const uint8_t variable_pitch_offset)
    {
        return getInstr()->setVariablePitchOffset(variable_pitch_offset);
    }
    uint8_t InstrumentRack::getVariablePitchOffset() const
    {
        return getInstr()->getVariablePitchOffset();
    }
    void InstrumentRack::kill(const bool kill)
    {
        this->_kill = kill;
        for (InstrumentBase* instr_ptr : this->instruments)
        {
            instr_ptr->kill(kill);
        }
    }
    InstrumentBase * InstrumentRack::getInstr()
    {
        return instruments[selection % instruments.size()];
    }
    InstrumentBase * InstrumentRack::getInstr() const
    {
        return instruments[selection % instruments.size()];
    }
};
