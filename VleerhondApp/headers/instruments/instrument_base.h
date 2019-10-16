#pragma once

#include "ofMain.h"

#include "timing_structs.h"
#include "midi_channel.h"
#include "harmony_struct.h"

namespace Vleerhond
{
    class InstrumentBase
    {
    protected:
        TimeStruct& time;
        MicroTimingStruct timing;

        uint32_t last_randomized_time;
        bool randomizable;
        bool active;
        std::vector<CcParam> params;

    public:
        MidiChannel midi_channel;
        bool kill;

        InstrumentBase(TimeStruct& time_ref, const bool is_randomizable) :
            time(time_ref)
        {
            kill = false;
            randomizable = is_randomizable;
            active = true;
        }

        virtual bool play() = 0;
        virtual void randomize()
        {
            last_randomized_time = millis();
            randomize_parameters();
        }

        void randomize_parameters()
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

        void process_events()
        {
            midi_channel.process_events();
        }

        void stop_notes()
        {
            midi_channel.all_notes_off();
        }

        uint32_t randomized_time()
        {
            return last_randomized_time;
        }

        bool is_randomizable()
        {
            return randomizable;
        }

        virtual uint8_t get_velocity()
        {
            return 100;
        }

        bool is_active() const
        {
            return this->active;
        }

        void set_active(const bool active)
        {
            this->active = active;
        }
    };

    class TonalInstrumentBase : public InstrumentBase
    {
    protected:
        HarmonyStruct& harmony;

    public:
        TonalInstrumentBase(HarmonyStruct& harmony, TimeStruct& time, const bool is_randomizable) :
            InstrumentBase(time, is_randomizable),
            harmony(harmony)
        {
        }

    };
}