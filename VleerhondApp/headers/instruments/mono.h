#pragma once

#include "instruments/instrument_base.h"

#include "patterns/cv_patterns.h"
#include "patterns/gate_patterns.h"
#include "patterns/interval_pattern.h"
#include "patterns/arp.h"
#include "patterns/modulators.h"
#include "patterns/sample_and_hold.h"

namespace Vleerhond
{
    enum class MonoPitchMode {
        ARP,
        SEQUENCE
    };

    class NoteInfo
    {
    public:
        bool hit;
        uint64_t tick;
        NoteStruct note;
    };

    class Mono : public TonalInstrumentBase
    {
    protected:
        NoteRepeat note_repeat_sh;

        ArpData arp_data;
        TimeDivision arp_reset_interval;

        GatePatternAB gate_pattern;
        IntervalPattern lead_pattern;
        CvPatternAB pitch_pattern;
        CvPatternAB octave_pattern;
        uint8_t octave_range;
        GatePatternAB slide_pattern;
        GatePatternAB accent_pattern;

        MonoStyle style;
        MonoPitchMode pitch_mode = MonoPitchMode::ARP;

        NoteInfo note_event;

    public:
        uint8_t variable_pitch_offset;
        uint8_t variable_density;

        struct {
            uint8_t p_arp = 16;
            uint8_t p_euclid = 16;
        } settings;

        Mono(
            HarmonyStruct& harmony,
            Modulators& modulators,
            TimeStruct& time,
            const uint8_t midi_channel
        );

        void randomize_arp();
        void randomize_rhythm();
        void randomize_chaos();
        virtual void randomize();
        void total_randomize();
        bool get_hit() const;
        uint8_t get_sequence_pitch() const;
        uint8_t get_next_mono_pitch();
        uint8_t get_mono_pitch() const;
        NoteInfo get_note_event();
        bool play();
        void check_arp_reset();
        void set_arp_type(ArpType arp_type);
        void set_style(MonoStyle mono_style);
        void set_pitch_mode(const MonoPitchMode pitch_mode);
        virtual uint8_t get_velocity();
    };
}