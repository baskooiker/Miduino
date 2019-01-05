#pragma once

#include "defs.h"
#include "midi_io.h"
#include "pitch.h"
#include "scales.h"
#include "rand.h"
#include "randomize.h"

/*void root_rocket_seq(ApplicationData& data)
{
    CvPatternAB& p_pattern = data.settings_rocket.pitches;
    fill_bar(p_pattern.patterns[0], 0);
    fill_bar(p_pattern.patterns[1], 0);
    fill_bar(p_pattern.patterns[2], 0);

    fill_bar(data.settings_rocket.octaves.patterns[0], 3);
    fill_bar(data.settings_rocket.octaves.patterns[1], 3);
    fill_bar(data.settings_rocket.octaves.patterns[2], 3);

    set_random_pattern_ab(data.settings_rocket.slides, .25f);
    data.settings_rocket.accents = init_percussive_pattern_64();
    randomize_cv(data.settings_rocket.probs);
    randomize_cv(data.settings_rocket.variable_octaves);

    randomize_interval_hats(data.settings_rocket.int_pattern, arp_interval_probs);
}*/

void modify_rocket_seq(ApplicationData& data)
{
    CvPatternAB& p_pattern = data.settings_rocket.pitches;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < NOTES_IN_BAR; j++)
        {
            if (randf() < .25)
            {
                p_pattern.patterns[i][j] = data.scale.notes[data.scale.length];
            }
            if (randf() < .25)
            {
                data.settings_rocket.octaves.patterns[i][j] = randi(2, 5);
            }
        }
    }
    set_random_pattern_ab(data.settings_rocket.slides, .25f);
    data.settings_rocket.accents = init_percussive_pattern_64();
}

void randomize_octaves(CvPattern& pattern, uint8_t min, uint8_t max)
{
    for (uint8_t i = 0; i < NOTES_IN_BAR; i++)
    {
        pattern[i] = randi(min, max);
    }
}

void randomize_notes(CvPattern& pattern, uint8_t range)
{
    for (uint8_t i = 0; i < NOTES_IN_BAR; i++)
    {
        pattern[i] = randi(range);
    }
}

void randomize_rocket_seq(ApplicationData& data)
{
    CvPatternAB& p_pattern = data.settings_rocket.pitches;

    randomize_octaves(data.settings_rocket.octaves.patterns[0], 2, 4);
    randomize_octaves(data.settings_rocket.octaves.patterns[1], 2, 4);
    randomize_octaves(data.settings_rocket.octaves.patterns[2], 2, 4);
    set_ab_pattern(data.settings_rocket.octaves.abPattern);

    randomize_notes(data.settings_rocket.pitches.patterns[0], data.scale.length);
    randomize_notes(data.settings_rocket.pitches.patterns[1], data.scale.length);
    randomize_notes(data.settings_rocket.pitches.patterns[2], data.scale.length);
    set_ab_pattern(data.settings_rocket.pitches.abPattern);

    set_random_pattern_ab(data.settings_rocket.slides, .25f);

    randomize_cv(data.settings_rocket.probs);

    randomize_interval_hats(data.settings_rocket.int_pattern, arp_interval_probs);

    data.settings_rocket.accents = init_percussive_pattern_64();
    randomize_cv(data.settings_rocket.variable_octaves);
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
        if (data.settings_rocket.note_range != NoteRange::RangeRoot)
        {
            if (data.settings_rocket.note_range == NoteRange::RangeChord)
            {
                note_nr = to_chord_order(cv(rocket.pitches, data.step));
            }
            else
            {
                note_nr = cv(rocket.pitches, data.step);
            }
        }
        uint8_t harmony = cv(data.harmony.pitches, data.step);
        uint8_t octave = get_octave(rocket.octaves, data.step);
        uint8_t variable_octave = cv(data.settings_rocket.variable_octaves, data.step);
        if (variable_octave < data.settings_rocket.pitch_range)
        {
            octave += variable_octave % 3 + 1;
        }
        uint8_t pitch = apply_scale(note_nr + harmony, data.scale, octave);

        // Note length
        uint8_t length = 12;
        if (!gate(rocket.slides, data.step, data.ticks))
        {
            all_notes_off(data.settings_rocket.storage, MIDI_CHANNEL_ROCKET);
            length = 6;
        }

        // Play it!
        note_on(pitch, velocity, MIDI_CHANNEL_ROCKET, data.settings_rocket.storage, length);
    }
}
