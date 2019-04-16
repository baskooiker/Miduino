#pragma once

#include "gate_patterns.h"
#include "interval_pattern.h"
#include "arp.h"
#include "rand.h"
#include "sample_and_hold.h"

namespace Vleerhond
{
    class StrUtils
    {
    public:
        static const std::string get_string(const MonoStyle style)
        {
            switch (style)
            {
            case MonoStyle::MonoFugue:
                return "fugue";
            case MonoStyle::MonoLeadPattern:
                return "lead_pattern";
            case MonoStyle::MonoPolyRhythm:
                return "poly_rhythm";
            case MonoStyle::MonoSixteenths:
                return "sixteenths";
            default:
                return "invalid style";
            }
        }
    };

    class MonoSettings : public TonalInstrumentBase
    {
    protected:
        FugueSettings& fugue_settings;
        SampleAndHold subtract_sh;
        NoteRepeat note_repeat_sh;
        uint8_t hit_probability;

    public:
        MonoStyle style;
        ArpData arp_data;

        GatePatternAB gate_pattern;
        IntervalPattern lead_pattern;

        uint8_t variable_pitch_offset;
        uint8_t variable_density;
        TimeDivision arp_reset_interval;

        uint8_t pitch_offset;
        uint8_t fugue_id;

        MonoSettings(
            FugueSettings& fugue_settings_ref,
            HarmonyStruct& harmony_ref,
            TimeStruct& time_ref) :
            TonalInstrumentBase(harmony_ref, time_ref, false),
            fugue_settings(fugue_settings_ref),
            subtract_sh(TimeDivision::Sixteenth),
            note_repeat_sh(TimeDivision::Eighth)
        {
            style = MonoStyle::MonoSixteenths;
            fugue_id = 0;
            arp_reset_interval = TimeDivision::Whole;
        }

        void randomize()
        {
            ofLogNotice("mono", "randomize()");
            last_randomized_time = millis();

            this->pitch_offset = Rand::randui8(36, 48);
            this->arp_data.range = Rand::randui8(12, 36);

            switch (Rand::randui8(4))
            {
            case 0: this->arp_data.type = ArpType::UP; break;
            case 1: this->arp_data.type = ArpType::DOWN; break;
            case 2: this->arp_data.type = ArpType::UPDOWN; break;
            case 3: this->arp_data.type = ArpType::PICKING_IN; break;
            }

            switch (Rand::distribution(16, 16, 16))
            {
            case 0: this->style = MonoStyle::MonoSixteenths; break;
            case 1: this->style = MonoStyle::MonoPolyRhythm; break;
            case 2: this->style = MonoStyle::MonoLeadPattern; break;
            }

            this->gate_pattern.set_euclid(Rand::randui8(5, 8), 1);
            this->gate_pattern.time_division = TimeDivision::Sixteenth;

            this->lead_pattern.randomize_interval_lead();

            switch (Rand::distribution(16, 16, 16, 16))
            {
            case 0: arp_reset_interval = TimeDivision::Whole; break;
            case 1: arp_reset_interval = TimeDivision::Two; break;
            case 2: arp_reset_interval = TimeDivision::Four; break;
            case 3: arp_reset_interval = TimeDivision::Eight; break;
            }

            ofLogNotice("mono", "mono type = %s", StrUtils::get_string(this->style).c_str());

            hit_probability = Rand::randi8(100, 128);
            note_repeat_sh.prob = Rand::randui8(32);
        }

        bool get_mono_hit() const
        {
            bool hit = false;
            switch (this->style)
            {
            case MonoStyle::MonoSixteenths:
                hit = Utils::interval_hit(TimeDivision::Sixteenth, time);
                break;
            case MonoStyle::MonoPolyRhythm:
                hit = this->gate_pattern.gate(time);
                break;
            case MonoStyle::MonoLeadPattern:
                hit = this->lead_pattern.hit(time);
                break;
            }
            return hit;
        }

        uint8_t get_next_mono_pitch()
        {
            this->arp_data.min = Utils::rerange(this->variable_pitch_offset, 48, this->pitch_offset);
            uint8_t pitch = this->arp_data.get_next_arp_pitch(harmony.scale, harmony.get_chord_step(time));
            return pitch;
        }

        uint8_t get_mono_pitch() const
        {
            return this->arp_data.get_arp_pitch();
        }

        void play()
        {
            this->check_arp_reset();

            if (this->style == MonoStyle::MonoFugue && !this->kill)
            {
                return this->fugue_settings.play_fugue(
                    this->fugue_id,
                    harmony,
                    time,
                    this->storage);
            }

            NoteStruct repeat_note = note_repeat_sh.repeat_note(time);
            if (repeat_note.pitch > 0)
            {
                //ofLogNotice("Mono", "note %d", repeat_note.pitch);
                if (Utils::interval_hit(note_repeat_sh.get_interval(time), time) && !this->kill)
                {
                    ofLogNotice("Mono", "note_repeating: %d", repeat_note.pitch);
                    storage.note_on(repeat_note, time.get_shuffle_delay());
                    return;
                }
            }

            bool hit = this->get_mono_hit();

            if (style == MonoStyle::MonoLeadPattern)
            {
                hit &= subtract_sh.value(time) > hit_probability;
            }

            if (hit)
            {
                uint8_t pitch = this->get_next_mono_pitch();

                uint8_t length = 6;
                if (this->style == MonoStyle::MonoLeadPattern)
                {
                    length = time.ticks_left_in_bar();
                }

                if (!this->kill)
                {
                    NoteStruct note = NoteStruct(pitch, 64, length, NoteType::Tie);
                    note_repeat_sh.set_repeat_note(note);
                    this->storage.note_on(note, time.get_shuffle_delay());
                }
            }
        }

        void check_arp_reset()
        {
            if (Utils::interval_hit(this->arp_reset_interval, time))
            {
                arp_data.counter = 0;
            }
        }
    };
}