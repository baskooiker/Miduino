#include "instrument_rack.h"

namespace Vleerhond
{
    InstrumentRack::InstrumentRack(TimeStruct & time)
        : InstrumentBase(time)
    {
    }

    bool InstrumentRack::play()
    {
        return getInstr()->play();
    }
    void InstrumentRack::randomize()
    {
        for (InstrumentBase* instrument : instruments)
        {
            instrument->randomize();
        }
    }
    std::vector<InstrumentBase*> InstrumentRack::get_ptrs()
    {
        return { getInstr() };
    }
    void InstrumentRack::auto_randomize()
    {
        return getInstr()->auto_randomize();
    }
    void InstrumentRack::select(const uint8_t index)
    {
        getInstr()->stop_notes();
        getInstr()->kill(false);
        this->selection = index;
    }
    void InstrumentRack::setVariableDensity(const uint8_t variable_density)
    {
        for (InstrumentBase* instrument : instruments)
        {
            instrument->setVariableDensity(variable_density);
        }
    }
    uint8_t InstrumentRack::getVariableDensity() const
    {
        return getInstr()->getVariableDensity();
    }
    void InstrumentRack::setVariablePitch(const uint8_t variable_pitch)
    {
        for (InstrumentBase* instrument : instruments)
        {
            instrument->setVariablePitch(variable_pitch);
        }
    }
    uint8_t InstrumentRack::getVariablePitch() const
    {
        return getInstr()->getVariablePitch();
    }
    void InstrumentRack::setVariablePitchOffset(const uint8_t variable_pitch_offset)
    {
        for (InstrumentBase* instrument : instruments)
        {
            instrument->setVariablePitchOffset(variable_pitch_offset);
        }
    }
    uint8_t InstrumentRack::getVariablePitchOffset() const
    {
        return getInstr()->getVariablePitchOffset();
    }
    void InstrumentRack::setVariableOctave(const uint8_t variable_octave)
    {
        for (InstrumentBase* instrument : instruments)
        {
            instrument->setVariableOctave(variable_octave);
        }
    }
    uint8_t InstrumentRack::getVariableOctave() const
    {
        return getInstr()->getVariableOctave();
    }
    void InstrumentRack::kill(const bool kill)
    {
        this->_kill = kill;
        for (InstrumentBase* instr_ptr : this->instruments)
        {
            instr_ptr->kill(kill);
        }
    }
    void InstrumentRack::setChannel(std::shared_ptr<MidiChannel> channel)
    {
        for (InstrumentBase* instr_ptr : this->instruments)
        {
            instr_ptr->setChannel(channel);
        }
    }
    std::shared_ptr<MidiChannel> InstrumentRack::getChannel()
    {
        return getInstr()->getChannel();
    }
    bool InstrumentRack::getPedal()
    {
        return getInstr()->getPedal();
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
