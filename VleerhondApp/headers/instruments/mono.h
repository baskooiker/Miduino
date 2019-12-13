#pragma once

#include "cv_patterns.h"
#include "gate_patterns.h"
#include "interval_pattern.h"
#include "arp.h"
#include "rand.h"
#include "sample_and_hold.h"
#include "instrument_base.h"

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

        MonoStyle style;
        MonoPitchMode pitch_mode = MonoPitchMode::ARP;

        NoteInfo note_event;
        ModulationReceiver velocity_mod;

    public:
        uint8_t variable_pitch_offset;
        uint8_t variable_density;

        struct {
            uint8_t p_arp = 16;
            uint8_t p_euclid = 16;
            uint8_t p_coef = 0;
        } settings;

        Mono(
            HarmonyStruct& harmony,
            Modulators& modulators,
            TimeStruct& time,
            const uint8_t midi_channel) :
            TonalInstrumentBase(harmony, time, true, midi_channel),
            note_repeat_sh(TimeDivision::Eighth),
            velocity_mod(modulators)
        {
            style = MonoStyle::MonoSixteenths;
            arp_reset_interval = TimeDivision::Whole;
        }

        void randomize_arp()
        {
            this->arp_data.range = Rand::randui8(12, 48);

            switch (Rand::randui8(4))
            {
            case 0: this->arp_data.type = ArpType::UP; break;
            case 1: this->arp_data.type = ArpType::DOWN; break;
            case 2: this->arp_data.type = ArpType::UPDOWN; break;
            case 3: this->arp_data.type = ArpType::PICKING_IN; break;
            }

            // Pitch patterns
            pitch_pattern.randomize();
            switch (Rand::distribution(16, 16))
            {
            case 0: pitch_pattern.length = 8; break;
            case 1: pitch_pattern.length = 16; break;
            }
            octave_pattern.randomize();
            switch (Rand::distribution(16, 16))
            {
            case 0: octave_pattern.length = 8; break;
            case 1: octave_pattern.length = 16; break;
            }

            octave_range = Rand::randi8(1, 4);
        }

        void randomize_rhythm()
        {
            // Set Euclid
            uint8_t length = Rand::distribution(16, 16) == 1 ? 16 : 8;
            uint8_t euclid_steps1 = Rand::randui8(length * 4, length * 3 / 4);
            uint8_t euclid_steps2 = Rand::randui8(length * 4, length * 3 / 4);
            if (Rand::distribution(32, 16) == 1)
            {
                euclid_steps2 = euclid_steps1;
            }
            this->gate_pattern.set_euclid(length, euclid_steps1);
            this->gate_pattern.patterns[1].set_euclid(length, euclid_steps2);
            this->gate_pattern.length = length;
            if (Rand::distribution(64, 16) == 1)
            {
                gate_pattern.patterns[0].remove_one();
            }
            if (Rand::distribution(64, 16) == 1)
            {
                gate_pattern.patterns[1].remove_one();
            }
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

            note_repeat_sh.prob = Rand::randui8(16);
        }

        virtual void randomize()
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

            velocity_mod.randomize(64, 64, .5);
        }

        void total_randomize()
        {
            TonalInstrumentBase::randomize();

            randomize_arp();
            randomize_rhythm();
            randomize_chaos();

            switch (Rand::distribution(
                settings.p_arp, 
                settings.p_euclid, 
                settings.p_coef))
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
            if (pitch_mode == MonoPitchMode::SEQUENCE)
            {
                uint8_t pitch = harmony.scale.get_penta(this->pitch_pattern.value(time));
                pitch = Utils::clip_pitch(pitch, variable_pitch_offset);
                uint8_t octave = Utils::rerange(octave_pattern.value(time), this->octave_range);
                pitch += octave * 12;
                return pitch;
            }
            // Else MonoPitchMode::ARP
            return this->arp_data.get_arp_pitch();
        }

        NoteInfo get_note_event()
        {
            if (time.tick != note_event.tick)
            {
                note_event.tick = time.tick;
                note_event.hit = get_hit();
                if (note_event.hit)
                {
                    uint8_t pitch = this->get_mono_pitch();

                    uint8_t length = 6;
                    if (this->style == MonoStyle::MonoLeadPattern)
                    {
                        length = time.ticks_left_in_bar();
                    }

                    note_event.note = NoteStruct(pitch, 64, length, NoteType::Tie);
                    note_repeat_sh.set_repeat_note(note_event.note);
                }
            }
            return note_event;
        }

        bool play()
        {
            this->check_arp_reset();

            if (this->kill)
            {
                return false;
            }

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

            NoteInfo new_note_event = get_note_event();
            if (new_note_event.hit)
            {
                this->midi_channel.note_on(new_note_event.note, time.get_shuffle_delay());
                return true;
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

        uint8_t get_velocity()
        {
            uint8_t value = 0;
            this->velocity_mod.value(time, value);
            return value;
        }

    };
}