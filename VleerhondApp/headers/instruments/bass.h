#pragma once

#include "cv_patterns.h"
#include "euclid.h"
#include "gate_patterns.h"
#include "interval_pattern.h"
#include "harmony_struct.h"
#include "fugue.h"
#include "instrument_base.h"
#include "sample_and_hold.h"

namespace Vleerhond
{
    class Bass : public TonalInstrumentBase
    {
    protected:
        SampleAndHold octave_sh;

    public:
        GatePatternAB accents;
        CvPatternAB pitches;
        CvPatternAB octaves;
        CvPatternAB variable_octaves;
        GatePatternAB slides;

        CvPatternAB probs;
        IntervalPattern int_pattern;
        GatePatternAB euclid_pattern;

        uint8_t pitch_range;
        BassStyle style;
        CvPatternAB note_range_prob;
        uint8_t note_range_value;
        uint8_t density;

        Bass(
            HarmonyStruct& harmony_ref,
            TimeStruct& time_ref) :
            TonalInstrumentBase(harmony_ref, time_ref, true),
            octave_sh(TimeDivision::Sixteenth)
        {
            pitch_range = 0;
            style = BassStyle::BassEuclid;
            note_range_value = 0;
            density = 0;
            octave_sh.prob = 16;

            total_randomize();
        }

        void randomize_octaves()
        {
            this->octaves.randomize(2, Rand::randui8(4, 6));
            this->octaves.patterns[0].set(0, 0);
            switch (Rand::distribution(16, 16, 16, 16))
            {
            case 0: this->octaves.length = 2; break;
            case 1: this->octaves.length = 4; break;
            case 2: this->octaves.length = 8; break;
            case 3: this->octaves.length = 16; break;
            }
            this->variable_octaves.randomize();
            switch (Rand::distribution(16, 16, 16, 16))
            {
            case 0: this->variable_octaves.length = 2; break;
            case 1: this->variable_octaves.length = 4; break;
            case 2: this->variable_octaves.length = 8; break;
            case 3: this->variable_octaves.length = 16; break;
            }
        }

        void randomize_pitches()
        {
            // Randomize pitches
            this->pitches.randomize();
            for (int i = 0; i < 3; i++)
            {
                this->pitches.patterns[i].set(0, 0);
            }

            uint8_t prob_2 = this->pitches.abPattern.isConstant() ? 0 : 32;
            switch (Rand::distribution(prob_2, 16, 16, 0))
            {
            case 0: this->pitches.length = 2; break;
            case 1: this->pitches.length = 4; break;
            case 2: this->pitches.length = 8; break;
            case 3: this->pitches.length = 16; break;
            }

            this->note_range_prob.randomize();
            switch (Rand::distribution(16, 16, 16, 16))
            {
            case 0: this->note_range_prob.length = 2; break;
            case 1: this->note_range_prob.length = 4; break;
            case 2: this->note_range_prob.length = 8; break;
            case 3: this->note_range_prob.length = 16; break;
            }
        }

        void randomize_gates()
        {
            // Randomize gates
            this->probs.randomize();
            switch (Rand::distribution(32, 16, 16))
            {
            case 0: this->probs.length = 4; break;
            case 1: this->probs.length = 8; break;
            case 2: this->probs.length = 16; break;
            }

            switch (Rand::distribution(16, 8, 16, 32))
            {
            case 0:
            {
                // Euclid length 16
                uint8_t steps = 3;
                switch (Rand::distribution(16, 16, 16, 16, 16))
                {
                case 0: steps = 5;  break;
                case 1: steps = 6;  break;
                case 2: steps = 7;  break;
                case 3: steps = 9;  break;
                case 4: steps = 11;  break;
                }
                this->euclid_pattern.set_euclid(16, steps);
                this->euclid_pattern.length = 16;
                style = BassStyle::BassEuclid;
                break;
            }
            case 1:
            {
                // Euclid length 8
                uint8_t steps = 3;
                switch (Rand::distribution(40, 20))
                {
                case 0: steps = 3;  break;
                case 1: steps = 4;  break;
                }
                this->euclid_pattern.set_euclid(8, steps);
                this->euclid_pattern.length = 8; 
                style = BassStyle::BassEuclid;
                break;
            }
            case 2:
            {
                // Settings interval pattern
                this->int_pattern.randomize_interval(arp_interval_probs);
                style = BassStyle::BassArpInterval;
                break;
            }
            case 3:
            {
                // Setting diddles
                euclid_pattern.set_diddles(Rand::randf(.5, .75), true);
                style = BassStyle::BassEuclid;
                break;
            }
            }

        }

        void randomize_accents()
        {
            this->slides.randomize(Rand::randf(.20f, .80f));
            this->accents.randomize(Rand::randf(.15f, .4f));
        }

        void randomize()
        {
            TonalInstrumentBase::randomize();

            octave_sh.prob = Rand::randui8(32);

            switch (Rand::distribution(16, 16, 16, 16))
            {
            case 0: randomize_octaves(); break;
            case 1: randomize_pitches(); break;
            case 2: randomize_gates(); break;
            case 3: randomize_accents(); break;
            }

        }

        void total_randomize()
        {
            TonalInstrumentBase::randomize();

            octave_sh.prob = Rand::randui8(32);

            randomize_octaves();
            randomize_pitches();
            randomize_gates();
            randomize_accents();
        }

        bool get_bass_hit(const uint8_t density, const TimeStruct& time)
        {
            bool hit = false;
            switch (this->style)
            {
            case BassStyle::BassEuclid:
                hit = this->euclid_pattern.gate(time);
                break;
            case BassStyle::BassArpInterval:
                hit = this->int_pattern.hit(time);
                break;
            }

            uint8_t prob = this->probs.value(time);
            bool prob_step = (prob < density) && (prob > 0) && Utils::interval_hit(TimeDivision::Sixteenth, time);
            return hit || prob_step;
        }

        uint8_t get_bass_pitch(
            const uint8_t variable_pitch,
            const uint8_t note_offset)
        {
            // TODO: Hier klopt dus niks van...
            uint8_t note_nr = 0;
            uint8_t note_range_p = this->note_range_prob.value(time);

            // TODO: Deze geeft veel te vaak false.
            if (note_range_p < this->note_range_value)
            {
                uint8_t pitch_cv = harmony.scale.get_note(this->pitches.value(time));

                if (this->note_range_value < 64)
                {
                    note_nr = Utils::to_chord_order(pitch_cv);
                }
                else
                {
                    if (note_range_p % 64 < this->note_range_value % 64)
                    {
                        note_nr = pitch_cv;
                    }
                    else
                    {
                        note_nr = Utils::to_chord_order(pitch_cv);
                    }
                }
            }

            const uint8_t pitch_offset = 36;

            uint8_t pitch = harmony.scale.apply_scale_offset(
                note_nr,
                pitch_offset,
                harmony.get_chord_step(time) + note_offset
            );

            uint8_t octave = this->octaves.value(time);
            if (variable_pitch < this->pitch_range)
            {
                pitch += (variable_pitch % 3 + 1) * 12;
            }

            pitch = Utils::clip_pitch(pitch, pitch_offset, Utils::rerange(variable_pitch, pitch_offset + 12, 36));

            return pitch;
        }

        bool play()
        {
            if (this->kill)
            {
                return false;
            }

            // Get hit

            if (this->get_bass_hit(this->density, time))
            {
                uint8_t pitch = get_bass_pitch(
                    this->variable_octaves.value(time),
                    (uint8_t)NoteInterval::IntervalRoot
                );

                // Note length
                uint8_t length = this->accents.gate(time) ? 6 : 2;
                NoteType note_type = NoteType::Tie;
                if (this->slides.gate(time))
                {
                    length = time.ticks_left_in_bar();
                    length = TICKS_PER_STEP * 4;
                    note_type = NoteType::Slide;
                }
                else
                {
                    if (!this->get_bass_hit(this->density, time.add(TICKS_PER_STEP)))
                    {
                        length = TICKS_PER_STEP * 2;
                    }
                }

                // Sample and hold on random octave jumps
                if (octave_sh.gate(time))
                {
                    pitch += 12;
                }

                // Play it!
                this->midi_channel.note_on(
                    NoteStruct(pitch, this->get_velocity(), length, note_type),
                    time.get_shuffle_delay()
                );
                return true;
            }
            return false;
        }

    };
}