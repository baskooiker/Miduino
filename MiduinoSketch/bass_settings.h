#pragma once

#include "cv_patterns.h"
#include "euclid.h"
#include "gate_patterns.h"
#include "interval_pattern.h"
#include "harmony_struct.h"
#include "fugue.h"
#include "instrument_base.h"

class BassSettings : public TonalInstrumentBase
{
protected:
    FugueSettings& fugue_settings;

public:
    GatePatternAB accents;
    CvPatternAB pitches;
    CvPatternAB octaves;
    CvPatternAB variable_octaves;
    GatePatternAB slides;

    CvPatternAB probs;
    IntervalPattern int_pattern;
    GatePatternAB euclid_pattern;
    GatePatternAB low_pattern;

    uint8_t pitch_range;
    BassStyle style;
    CvPatternAB note_range_prob;
    uint8_t note_range_value;
    uint8_t fugue_id;
    uint8_t density;

    BassSettings(
        FugueSettings& fugue_settings_ref,
        HarmonyStruct& harmony_ref,
        TimeStruct& time_ref) :
        TonalInstrumentBase(harmony_ref, time_ref),
        fugue_settings(fugue_settings_ref)
    {
        pitch_range = 0;
        style = BassStyle::BassLow;
        note_range_value = 0;
        density = 0;
        kill = false;
    }

    void randomize()
    {
        last_randomized_time = millis();

        // Randomize octaves
        this->octaves.randomize(2, Rand::randui8(4, 6));
        this->octaves.patterns[0].set(0, 0);
        switch (Rand::distribution(16, 16, 16, 32))
        {
        case 0: this->octaves.length = 2; break;
        case 1: this->octaves.length = 4; break;
        case 2: this->octaves.length = 8; break;
        case 3: this->octaves.length = 16; break;
        }
        this->variable_octaves.randomize();
        switch (Rand::distribution(16, 16, 16, 32))
        {
        case 0: this->variable_octaves.length = 2; break;
        case 1: this->variable_octaves.length = 4; break;
        case 2: this->variable_octaves.length = 8; break;
        case 3: this->variable_octaves.length = 16; break;
        }

        // Randomize pitches
        this->pitches.randomize();
        for (int i = 0; i < 3; i++)
        {
            this->pitches.patterns[i].set(0, 0);
        }
        switch (Rand::distribution(16, 16, 16, 32))
        {
        case 0: this->pitches.length = 2; break;
        case 1: this->pitches.length = 4; break;
        case 2: this->pitches.length = 8; break;
        case 3: this->pitches.length = 16; break;
        }
        this->note_range_prob.randomize();
        switch (Rand::distribution(16, 16, 16, 32))
        {
        case 0: this->note_range_prob.length = 2; break;
        case 1: this->note_range_prob.length = 4; break;
        case 2: this->note_range_prob.length = 8; break;
        case 3: this->note_range_prob.length = 16; break;
        }

        // Randomize gates
        this->low_pattern.set_gates_low();
        this->probs.randomize();
        switch (Rand::distribution(16, 16, 32))
        {
        case 0: this->note_range_prob.length = 4; break;
        case 1: this->note_range_prob.length = 8; break;
        case 2: this->note_range_prob.length = 16; break;
        }

        // Randomize style
        switch (Rand::distribution(16, 16, 16))
        {
        case 0: this->style = BassStyle::BassArpInterval; break;
        case 1: this->style = BassStyle::BassEuclid; break;
        case 2: this->style = BassStyle::BassLow; break;
        }

        // Randomize euclid
        uint8_t steps = 5;
        uint8_t step_dist = Rand::distribution(20, 20);
        if (step_dist == 0)
        {
            switch (Rand::distribution(40, 20))
            {
            case 0: steps = 3;  break;
            case 1: steps = 5;  break;
            }
            this->euclid_pattern.set_euclid(16, steps);
            this->euclid_pattern.length = 16;
        }
        else if (step_dist == 1)
        {
            switch (Rand::distribution(40, 20))
            {
            case 0: steps = 3;  break;
            case 1: steps = 4;  break;
            }
            this->euclid_pattern.set_euclid(8, steps);
            this->euclid_pattern.length = 8;
        }

        // Randomize others
        this->int_pattern.randomize_interval(arp_interval_probs);
        this->slides.randomize(.15f);
        this->accents.randomize(Rand::randf(.15f, 1.f));
    }

    bool get_bass_hit(const uint8_t density, const TimeStruct& time)
    {
        bool hit = false;
        switch (this->style)
        {
        case BassStyle::BassLow:
            hit = this->low_pattern.gate(time);
            break;
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

    void play()
    {
        if (this->kill)
        {
            return;
        }

        if (this->style == BassStyle::BassFugue && (time.tick % TICKS_PER_STEP) == 0)
        {
            return this->fugue_settings.play_fugue(
                this->fugue_id,
                harmony,
                time,
                this->storage);
        }

        // Get hit
        bool hit = this->get_bass_hit(this->density, time);

        if (hit)
        {
            uint8_t pitch = get_bass_pitch(
                this->variable_octaves.value(time),
                NoteInterval::IntervalRoot
            );

            // Note length
            uint8_t length = this->accents.gate(time) ? 6 : 2;
            NoteType note_type = NoteType::Tie;
            if (this->slides.gate(time))
            {
                length = time.ticks_left_in_bar();
                note_type = NoteType::Tie;
            }

            // Play it!
            this->storage.note_on(
                NoteStruct(pitch, 64, length, note_type),
                time.get_shuffle_delay()
            );
        }
    }

};
