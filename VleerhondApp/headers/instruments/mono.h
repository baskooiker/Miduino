#pragma once

#include "gate_patterns.h"
#include "interval_pattern.h"
#include "arp.h"
#include "rand.h"
#include "sample_and_hold.h"
#include "instrument_base.h"

namespace Vleerhond
{
    class Mono : public TonalInstrumentBase
    {
    protected:
        SampleAndHold subtract_sh;
        NoteRepeat note_repeat_sh;

        ArpData arp_data;
        TimeDivision arp_reset_interval;

        GatePatternAB gate_pattern;
        IntervalPattern lead_pattern;

        MonoStyle style;

    public:
        uint8_t variable_pitch_offset;
        uint8_t variable_density;

        Mono(
            HarmonyStruct& harmony_ref,
            TimeStruct& time_ref) :
            TonalInstrumentBase(harmony_ref, time_ref, true),
            subtract_sh(TimeDivision::Sixteenth),
            note_repeat_sh(TimeDivision::Eighth)
        {
            style = MonoStyle::MonoSixteenths;
            arp_reset_interval = TimeDivision::Whole;

            total_randomize();
        }

        void randomize_arp()
        {
            this->arp_data.range = Rand::randui8(12, 36);

            switch (Rand::randui8(4))
            {
            case 0: this->arp_data.type = ArpType::UP; break;
            case 1: this->arp_data.type = ArpType::DOWN; break;
            case 2: this->arp_data.type = ArpType::UPDOWN; break;
            case 3: this->arp_data.type = ArpType::PICKING_IN; break;
            }
        }

        void randomize_rhythm()
        {
            // Set Euclid
            uint8_t euclid_length = Rand::randui8(5, 8);
            uint8_t euclid_steps = (euclid_length / 2) + Rand::randui8(2);
            this->gate_pattern.set_euclid(euclid_length, euclid_steps);
            this->gate_pattern.length = euclid_length;
            this->gate_pattern.time_division = TimeDivision::Sixteenth;

            // Randomize Lead
            this->lead_pattern.randomize_interval_lead();
        }

        void randomize_chaos()
        {
            switch (Rand::distribution(16, 16, 16, 16))
            {
            case 0: arp_reset_interval = TimeDivision::Whole; break;
            case 1: arp_reset_interval = TimeDivision::Two; break;
            case 2: arp_reset_interval = TimeDivision::Four; break;
            case 3: arp_reset_interval = TimeDivision::Eight; break;
            }

            subtract_sh.prob = Rand::randi8(16);
            note_repeat_sh.prob = Rand::randui8(16);
        }

        void randomize()
        {
            ofLogVerbose("mono", "randomize()");
            TonalInstrumentBase::randomize();

            switch (Rand::distribution(16, 16, 16))
            {
            case 0:
                randomize_arp();
                break;
            case 1:
                randomize_rhythm();
                break;
            case 2:
                randomize_chaos();
                break;
            }
        }

        void total_randomize()
        {
            TonalInstrumentBase::randomize();

            randomize_arp();
            randomize_rhythm();
            randomize_chaos();

            switch (Rand::distribution(16, 16, 16))
            {
            case 0: this->style = MonoStyle::MonoSixteenths; break;
            case 1: this->style = MonoStyle::MonoPolyRhythm; break;
            case 2: this->style = MonoStyle::MonoLeadPattern; break;
            }
        }

        bool get_hit() const
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
            this->arp_data.min = Utils::rerange(this->variable_pitch_offset, 48, 36);
            uint8_t pitch = this->arp_data.get_next_arp_pitch(harmony.scale, harmony.get_chord_step(time));
            return pitch;
        }

        uint8_t get_mono_pitch() const
        {
            return this->arp_data.get_arp_pitch();
        }

        bool play()
        {
            this->check_arp_reset();

            NoteStruct repeat_note = note_repeat_sh.repeat_note(time);
            if (repeat_note.pitch > 0)
            {
                TimeDivision repeat_interval = note_repeat_sh.get_interval(time);
                ofLogVerbose("Mono", "note %3d, interval = %d", repeat_note.pitch, repeat_interval);
                if (Utils::interval_hit(repeat_interval, time) && !this->kill)
                {
                    ofLogVerbose("Mono", "note_repeating: %d", repeat_note.pitch);
                    midi_channel.note_on(repeat_note, time.get_shuffle_delay());
                    return true;
                }
            }

            bool hit = this->get_hit();

            if (style == MonoStyle::MonoLeadPattern)
            {
                hit &= !subtract_sh.gate(time);
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
                    this->midi_channel.note_on(note, time.get_shuffle_delay());
                    return true;
                }
            }
            return false;
        }

        void check_arp_reset()
        {
            if (Utils::interval_hit(this->arp_reset_interval, time))
            {
                arp_data.counter = 0;
            }
        }

        void set_arp_type(ArpType arp_type)
        {
            arp_data.type = arp_type;
        }

        void set_style(MonoStyle mono_style)
        {
            style = mono_style;
        }

    };
}