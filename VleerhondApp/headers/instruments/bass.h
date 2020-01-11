#pragma once

#include <sstream>

#include "harmony/harmony_struct.h"
#include "instruments/instrument_base.h"
#include "patterns/cv_patterns.h"
#include "patterns/euclid.h"
#include "patterns/gate_patterns.h"
#include "patterns/interval_pattern.h"
#include "patterns/fugue.h"
#include "patterns/sample_and_hold.h"

namespace Vleerhond
{
    class Bass : public TonalInstrumentBase
    {
    protected:
        SampleAndHold octave_sh;

    public:
        struct
        {
            struct
            {
                uint8_t p_5 = 16;
                uint8_t p_6 = 16;
                uint8_t p_7 = 16;
                uint8_t p_9 = 16;
                uint8_t p_11 = 16;
            } euclid_16;
            uint8_t p_euclid_16 = 16;

            struct
            {
                uint8_t p_3 = 40;
                uint8_t p_5 = 20;
                uint8_t p_7 = 20;
            } euclid_8;
            uint8_t p_euclid_8 = 8;

            uint8_t p_interval = 16;

            struct
            {
                double p_min = .5;
                double p_max = .75;
            } diddles;
            uint8_t p_diddles = 32;
            uint8_t p_octave_sh = 32;
        } settings;

    public:
        GatePatternAB accents;
        CvPatternAB pitches;
        CvPatternAB octaves;
        CvPatternAB variable_octaves;
        GatePatternAB slides;

        CvPatternAB probs;
        IntervalPattern int_pattern;
        GatePatternAB euclid_pattern;

        BassStyle style;
        CvPatternAB note_range_prob;
        uint8_t note_range_value;
        uint8_t density;
        uint8_t variable_octave = 0;
        uint8_t pitch_offset = 36;
        bool follow_harmony = true;

        Bass(
            HarmonyStruct& harmony_ref,
            TimeStruct& time_ref,
            const uint8_t midi_channel,
            const int8_t offset = 0
        );
        void randomize_octaves();
        void randomize_pitches();
        void randomize_drop();
        void randomize_gates();
        void randomize_accents();
        void randomize();
        void total_randomize();
        uint8_t get_length();
        NoteType get_note_type();
        bool get_hit(const uint8_t density, const TimeStruct& time);
        uint8_t get_pitch();
        bool play();
        std::string toString();

    };
}
