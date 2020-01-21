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
        const uint8_t midi_channel,
        const int8_t offset) :
        time(time_ref),
        midi_channel(midi_channel, offset)
    {
        kill = false;
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

    void InstrumentBase::process_events()
    {
        midi_channel.process_events();
    }

    void InstrumentBase::stop_notes()
    {
        midi_channel.all_notes_off();
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

//////////////////////////////////////////

    TonalInstrumentBase::TonalInstrumentBase(
        HarmonyStruct& harmony, 
        TimeStruct& time, 
        const bool is_randomizable,
        const uint8_t midi_channel,
        const int8_t offset) :
        InstrumentBase(time, is_randomizable, midi_channel, offset),
        harmony(harmony)
    {
    }
}