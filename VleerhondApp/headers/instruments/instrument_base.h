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

        bool _kill = false;
        uint8_t _variable_density = 0;
        uint8_t _variable_pitch = 0;
        uint8_t _variable_pitch_offset = 64;

    public:
        MidiChannel midi_channel;

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
        void kill(const bool kill);
        bool isKilled() const;

        virtual void setVariableDensity(const uint8_t variable_density);
        virtual uint8_t getVariableDensity() const;
        virtual void setVariablePitch(const uint8_t variable_pitch);
        virtual uint8_t getVariablePitch() const;
        virtual void setVariablePitchOffset(const uint8_t variable_pitch_offset);
        virtual uint8_t getVariablePitchOffset() const;
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