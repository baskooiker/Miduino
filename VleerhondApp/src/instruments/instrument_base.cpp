#include "instruments/instrument_base.h"

#include "ofMain.h"

#include "core/timing_structs.h"
#include "harmony/harmony_struct.h"
#include "midi/dummy_midi_channel.h"
#include "midi/midi_channel.h"
#include "midi/midi_io.h"

namespace Vleerhond
{

    InstrumentBase::InstrumentBase(
        TimeStruct& time_ref) :
        time(time_ref),
        midi_channel(std::make_shared<DummyMidiChannel>())
    {
    }

    void InstrumentBase::randomize()
    {
        last_randomized_time = Utils::millis();
        randomize_parameters();
    }

    void InstrumentBase::randomize_parameters()
    {
        for (CcParam param : params)
        {
            MidiIO::send_cc(
                param.note,
                Rand::randui8(param.max, param.min),
                param.channel
            );
        }
    }

    void InstrumentBase::processNoteEvents()
    {
        getChannel()->processNoteEvents();
    }

    void InstrumentBase::stop_notes()
    {
        getChannel()->all_notes_off();
    }

    uint32_t InstrumentBase::randomized_time()
    {
        return last_randomized_time;
    }

    uint8_t InstrumentBase::get_velocity()
    {
        return 100;
    }

    std::vector<InstrumentBase*> InstrumentBase::get_ptrs()
    {
        return { this };
    }

    void InstrumentBase::check_auto_randomize()
    {
        uint64_t diff = Utils::millis() - randomized_time();
        if (diff > 60000)
        {
            if (Rand::distribution(diff / 1000, 240) == 0)
            {
                auto_randomize();
            }
        }
    }

    void InstrumentBase::auto_randomize()
    {
    }

    void InstrumentBase::kill(const bool kill)
    {
        this->_kill = kill;
    }

    bool InstrumentBase::isKilled() const
    {
        return this->_kill;
    }

    void InstrumentBase::setChannel(std::shared_ptr<MidiChannel> channel)
    {
        this->midi_channel = channel;
    }

    std::shared_ptr<MidiChannel> InstrumentBase::getChannel()
    {
        return this->midi_channel;
    }

    bool InstrumentBase::getPedal()
    {
        return false;
    }

    void InstrumentBase::updatePedalState()
    {
        getChannel()->set_pedal(this->getPedal());
    }

    void InstrumentBase::setVariableDensity(const uint8_t variable_density)
    {
        this->_variable_density = variable_density;
    }

    uint8_t InstrumentBase::getVariableDensity() const
    {
        return this->_variable_density;
    }

    void InstrumentBase::setVariablePitch(const uint8_t variable_pitch)
    {
        this->_variable_pitch = variable_pitch;
    }

    uint8_t InstrumentBase::getVariablePitch() const
    {
        return this->_variable_pitch;
    }

    void InstrumentBase::setVariablePitchOffset(const uint8_t variable_pitch_offset)
    {
        this->_variable_pitch_offset = variable_pitch_offset;
    }

    uint8_t InstrumentBase::getVariablePitchOffset() const
    {
        return this->_variable_pitch_offset;
    }

    void InstrumentBase::setVariableOctave(const uint8_t variable_octave)
    {
        this->_variable_octave = variable_octave;
    }

    uint8_t InstrumentBase::getVariableOctave() const 
    { 
        return this->_variable_octave;
    }

//////////////////////////////////////////

    TonalInstrumentBase::TonalInstrumentBase(
        HarmonyStruct& harmony, 
        TimeStruct& time) 
        : InstrumentBase(time)
        , harmony(harmony)
    {
    }
}