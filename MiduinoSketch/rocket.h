#pragma once

#include "defs.h"
#include "midi_io.h"
#include "pitch.h"
#include "scales.h"
#include "rand.h"

void randomize_rocket_seq(ApplicationData& data)
{
    CvPatternAB& p_pattern = data.settings_rocket.pitches;

    randomize(data.settings_rocket.octaves, 2, 4);
    randomize(data.settings_rocket.pitches, data.scale.length);
    set_gates_low(data.settings_rocket.low_pattern, 1);
    set_euclid(data.settings_rocket.euclid_pattern, 16, 5);
    
    randomize(data.settings_rocket.slides, .15f);

    randomize(data.settings_rocket.probs);
    randomize(data.settings_rocket.note_range_prob);

    randomize_interval(data.settings_rocket.int_pattern, arp_interval_probs);

    randomize(data.settings_rocket.accents, .5f);
    randomize(data.settings_rocket.variable_octaves);
}

void play_rocket(ApplicationData& data)
{
    NoteStruct note = { 0 };

    // Velocity
    note.velocity = data.settings_rocket.low_velocity;

    // Get hit
    bool hit = false;
    switch (data.settings_rocket.style)
    {
    //case RocketStyle::RocketWhole:
        //hit = interval_hit(TimeDivision::TIME_DIVISION_WHOLE, data.step, data.ticks);
        //break;
    case RocketStyle::RocketLow:
        hit = gate(data.settings_rocket.low_pattern, data.step, data.ticks);
        break;
    case RocketStyle::RocketEuclid:
        hit = gate(data.settings_rocket.euclid_pattern, data.step, data.ticks);
        break;
    case RocketStyle::RocketArpInterval:
        hit = interval_hit(data.settings_rocket.int_pattern, data.step, data.ticks);
        break;
    /*case RocketStyle::RocketProb:
        hit = (cv(data.settings_rocket.probs, data.step) <= (uint8_t)MIN(data.settings_rocket.gate_density, 127)) && (data.ticks % 6 == 0);
        break;*/
    case RocketStyle::RocketSixteenths:
        hit = interval_hit(TimeDivision::TIME_DIVISION_SIXTEENTH, data.step, data.ticks); 
        break;
    default: break;
    }

    if (hit)
    {
        // Pitch
        uint8_t note_nr = 0;
        uint8_t note_range_p = cv(data.settings_rocket.note_range_prob, data.step);
        if (note_range_p < data.settings_rocket.note_range_value)
        {
            if (data.settings_rocket.note_range_value < 64)
            {
                note_nr = to_chord_order(cv(data.settings_rocket.pitches, data.step));
            }
            else
            {
                if (note_range_p % 64 < data.settings_rocket.note_range_value % 64)
                {
                    note_nr = cv(data.settings_rocket.pitches, data.step);
                }
                else
                {
                    note_nr = to_chord_order(cv(data.settings_rocket.pitches, data.step));
                }
            }
        }

        uint8_t harmony = cv(data.harmony, data.step);
        uint8_t octave = cv(data.settings_rocket.octaves, data.step);
        uint8_t variable_octave = cv(data.settings_rocket.variable_octaves, data.step);
        if (variable_octave < data.settings_rocket.pitch_range)
        {
            octave += variable_octave % 3 + 1;
        }

        note.pitch = apply_scale(note_nr + harmony, data.scale, octave);

        // Note length
        if (gate(data.settings_rocket.slides, data.step, data.ticks) 
            || data.settings_rocket.style == RocketStyle::RocketLow)
        {
            note.length = TICKS_IN_BAR;
        }
        else
        {
            all_notes_off(data.settings_rocket.storage, MIDI_CHANNEL_ROCKET);
            note.length = 5;
        }

        // Play it!
        note_on(note, MIDI_CHANNEL_ROCKET, data.settings_rocket.storage);
    }
}
