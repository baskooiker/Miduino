#pragma once

#include "basslines.h"
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
    SettingsRocket& rocket = data.settings_rocket;
  
    // Velocity
    uint8_t velocity = rocket.low_velocity;
    if (gate(rocket.accents, data.step, data.ticks))
    {
        velocity = rocket.high_velocity;
    }

    // Get hit
    bool hit = false;
    switch (rocket.style)
    {
    case RocketStyle::RocketWhole:
        hit = interval_hit(TimeDivision::TIME_DIVISION_WHOLE, data.step, data.ticks);
        break;
    case RocketEuclid:
        hit = gate(rocket.euclid_pattern, data.step, data.ticks);
        break;
    case RocketStyle::RocketArpInterval:
        hit = interval_hit(rocket.int_pattern, data.step, data.ticks);
        break;
    case RocketStyle::RocketProb:
        hit = (cv(rocket.probs, data.step) <= (uint8_t)MIN(rocket.gate_density, 127)) && (data.ticks % 6 == 0);
        break;
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
                note_nr = to_chord_order(cv(rocket.pitches, data.step));
            }
            else
            {
                if (note_range_p % 64 < data.settings_rocket.note_range_value % 64)
                {
                    note_nr = cv(rocket.pitches, data.step);
                }
                else
                {
                    note_nr = to_chord_order(cv(rocket.pitches, data.step));
                }
            }
        }

        uint8_t harmony = cv(data.harmony, data.step);
        uint8_t octave = get_octave(rocket.octaves, data.step);
        uint8_t variable_octave = cv(data.settings_rocket.variable_octaves, data.step);
        if (variable_octave < data.settings_rocket.pitch_range)
        {
            octave += variable_octave % 3 + 1;
        }
        uint8_t pitch = apply_scale(note_nr + harmony, data.scale, octave);

        // Note length
        uint8_t length = 6;
        if (gate(rocket.slides, data.step, data.ticks))
        {
            length = TIE_NOTE;
        }
        else
        {
            all_notes_off(data.settings_rocket.storage, MIDI_CHANNEL_ROCKET);
            length = 6;
        }

        // Play it!
        note_on(pitch, velocity, MIDI_CHANNEL_ROCKET, data.settings_rocket.storage, length);
    }
}
