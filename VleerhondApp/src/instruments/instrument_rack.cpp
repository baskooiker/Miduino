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
    std::vector<InstrumentBase*> InstrumentRack::getPtrs()
    {
        return { getInstr() };
    }
    void InstrumentRack::autoRandomize()
    {
        return getInstr()->autoRandomize();
    }
    void InstrumentRack::select(const uint8_t index)
    {
        getInstr()->stopNotes();
        getInstr()->kill(false);
        this->selection = index;
    }
    void InstrumentRack::setVariableDensity(const uint8_t variable_density)
    {
        getInstr()->setVariableDensity(variable_density);
    }
    uint8_t InstrumentRack::getVariableDensity() const
    {
        return getInstr()->getVariableDensity();
    }
    void InstrumentRack::setVariablePitch(const uint8_t variable_pitch)
    {
        getInstr()->setVariablePitch(variable_pitch);
    }
    uint8_t InstrumentRack::getVariablePitch() const
    {
        return getInstr()->getVariablePitch();
    }
    void InstrumentRack::setVariablePitchOffset(const uint8_t variable_pitch_offset)
    {
        getInstr()->setVariablePitchOffset(variable_pitch_offset);
    }
    uint8_t InstrumentRack::getVariablePitchOffset() const
    {
        return getInstr()->getVariablePitchOffset();
    }
    void InstrumentRack::setVariableOctave(const uint8_t variable_octave)
    {
        getInstr()->setVariableOctave(variable_octave);
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
        //std::cout << "InstrumentRack::setChannel\n";
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
    
    std::string InstrumentRack::getName()
    {
        std::string n = name;
        n.append(":");
        n.append(getInstr()->getName());
        return n;
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
