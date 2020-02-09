#pragma once

#include "ofMain.h"

#include "core/timing_structs.h"
#include "harmony/harmony_struct.h"
#include "midi/midi_channel.h"

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

        InstrumentBase(
            TimeStruct& time_ref,
            const bool is_randomizable,
            const uint8_t midi_channel,
            const int8_t offset = 0
        );

        virtual bool play() = 0;
        virtual void randomize();
        void randomize_parameters();
        void processNoteEvents();
        void stop_notes();
        uint32_t randomized_time();
        bool is_randomizable();
        virtual uint8_t get_velocity();
        bool is_active() const;
        void set_active(const bool active);
        virtual std::vector<InstrumentBase*> get_ptrs();
        virtual void check_auto_randomize();
        virtual void auto_randomize();

    };

    class TonalInstrumentBase : public InstrumentBase
    {
    protected:
        HarmonyStruct& harmony;

    public:
        TonalInstrumentBase(
            HarmonyStruct& harmony,
            TimeStruct& time,
            const bool is_randomizable,
            const uint8_t midi_channel,
            const int8_t offset = 0
        );

    };
}