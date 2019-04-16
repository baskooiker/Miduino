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

    public:
        PitchStorage storage;
        bool kill;

        InstrumentBase(TimeStruct& time_ref, const bool is_randomizable) :
            time(time_ref)
        {
            kill = false;
            randomizable = is_randomizable;
        }

        virtual void play() = 0;
        virtual void randomize() = 0;

        void process_events()
        {
            storage.process_events();
        }

        void stop_notes()
        {
            storage.all_notes_off();
        }

        uint32_t randomized_time()
        {
            return last_randomized_time;
        }

        bool is_randomizable()
        {
            return randomizable;
        }
    };

    class TonalInstrumentBase : public InstrumentBase
    {
    protected:
        HarmonyStruct& harmony;

    public:
        TonalInstrumentBase(HarmonyStruct& harmony_ref, TimeStruct& time_ref, const bool is_randomizable) :
            InstrumentBase(time_ref, is_randomizable),
            harmony(harmony_ref)
        {
        }
    };
}