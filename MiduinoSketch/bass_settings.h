#pragma once

#include "cv_patterns.h"
#include "euclid.h"
#include "gate_patterns.h"

class BassSettings
{
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
    uint8_t octave_offset;
    uint8_t fugue_id;
    uint8_t density;

    bool kill;

    PitchStorage storage;

    BassSettings()
    {
        pitch_range = 0;
        style = BassStyle::BassLow;
        note_range_value = 0;
        density = 0;
        octave_offset = 2;
        kill = false;
    }

    void randomize()
    {
        // Randomize octaves
        this->octaves.randomize(2, randui8(4, 6));
        switch (distribution(16, 16, 16, 32))
        {
        case 0: this->octaves.length = 2; break;
        case 1: this->octaves.length = 4; break;
        case 2: this->octaves.length = 8; break;
        case 3: this->octaves.length = 16; break;
        }
        this->variable_octaves.randomize();
        switch (distribution(16, 16, 16, 32))
        {
        case 0: this->variable_octaves.length = 2; break;
        case 1: this->variable_octaves.length = 4; break;
        case 2: this->variable_octaves.length = 8; break;
        case 3: this->variable_octaves.length = 16; break;
        }

        // Randomize pitches
        this->pitches.randomize();
        switch (distribution(16, 16, 16, 32))
        {
        case 0: this->pitches.length = 2; break;
        case 1: this->pitches.length = 4; break;
        case 2: this->pitches.length = 8; break;
        case 3: this->pitches.length = 16; break;
        }
        this->note_range_prob.randomize();
        switch (distribution(16, 16, 16, 32))
        {
        case 0: this->note_range_prob.length = 2; break;
        case 1: this->note_range_prob.length = 4; break;
        case 2: this->note_range_prob.length = 8; break;
        case 3: this->note_range_prob.length = 16; break;
        }

        // Randomize gates
        this->low_pattern.set_gates_low();
        this->probs.randomize();
        switch (distribution(16, 16, 32))
        {
        case 0: this->note_range_prob.length = 4; break;
        case 1: this->note_range_prob.length = 8; break;
        case 2: this->note_range_prob.length = 16; break;
        }

        // Randomize style
        switch (distribution(16, 16, 16))
        {
        case 0: this->style = BassStyle::BassArpInterval; break;
        case 1: this->style = BassStyle::BassEuclid; break;
        case 2: this->style = BassStyle::BassLow; break;
        }

        // Randomize euclid
        uint8_t steps = 5;
        uint8_t step_dist = distribution(20, 20);
        if (step_dist == 0)
        {
            switch (distribution(40, 20))
            {
            case 0: steps = 3;  break;
            case 1: steps = 5;  break;
            }
            set_euclid(this->euclid_pattern, 16, steps);
            this->euclid_pattern.length = 16;
        }
        else if (step_dist == 1)
        {
            switch (distribution(40, 20))
            {
            case 0: steps = 3;  break;
            case 1: steps = 4;  break;
            }
            set_euclid(this->euclid_pattern, 8, steps);
            this->euclid_pattern.length = 8;
        }

        // Randomize others
        randomize_interval(this->int_pattern, arp_interval_probs);
        this->slides.randomize(.15f);
        this->accents.randomize(randf(.15f, 1.f));
        this->note_range_value = quad(randui8()) / 2;
    }

};
