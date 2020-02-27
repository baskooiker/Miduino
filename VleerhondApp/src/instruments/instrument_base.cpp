#include "instruments/instrument_base.h"

#include "ofMain.h"

#include "core/timing_structs.h"
#include "harmony/harmony_struct.h"
#include "midi/midi_channel.h"
#include "midi/midi_io.h"

namespace Vleerhond
{

    InstrumentBase::InstrumentBase(
        TimeStruct& time_ref, 
        const bool is_randomizable,
        const uint8_t midi_channel) :
        time(time_ref),
        midi_channel(std::make_shared<MidiChannel>(midi_channel))
    {
        _kill = false;
        randomizable = is_randomizable;
        active = true;
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

    bool InstrumentBase::is_randomizable()
    {
        return randomizable;
    }

    uint8_t InstrumentBase::get_velocity()
    {
        return 100;
    }

    bool InstrumentBase::is_active() const
    {
        return this->active;
    }

    void InstrumentBase::set_active(const bool active)
    {
        this->active = active;
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
        bool new_v = this->getPedal();
        //std::cout << "updatePedalState: " << new_v << ", channel " << (int)getChannel()->get_channel().channel << std::endl;
        //if (getChannel()->getPedal() != new_v)
        //{
            //std::cout << "== Switch pedal: " << (new_v ? "On" : "Off") << "\n";
        //}
        getChannel()->set_pedal(new_v);
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

//////////////////////////////////////////

    TonalInstrumentBase::TonalInstrumentBase(
        HarmonyStruct& harmony, 
        TimeStruct& time, 
        const bool is_randomizable,
        const uint8_t midi_channel) :
        InstrumentBase(time, is_randomizable, midi_channel),
        harmony(harmony)
    {
    }
}