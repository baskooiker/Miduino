#pragma once

#include "defs.h"
#include "midi_io.h"
#include "scales.h"
#include "rand.h"
#include "randomize.h"

void root_rocket_seq(ApplicationData& data)
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
}

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

    data.settings_rocket.accents = init_percussive_pattern_64();
}

void play_rocket(ApplicationData& data)
{
    if (data.ticks % TICKS_PER_STEP != 0)
    {
        return;
    }

    SettingsRocket& rocket = data.settings_rocket;
  
    uint8_t velocity = rocket.low_velocity;
    if (gate(rocket.accents, data.step, data.ticks))
    {
        velocity = rocket.high_velocity;
    }

    uint8_t note_nr = cv(rocket.pitches, data.step);
    uint8_t harmony = cv(data.harmony.pitches, data.step);

    uint8_t octave = get_octave(rocket.octaves, data.step);

    uint8_t variable_octave = cv(data.settings_rocket.variable_octaves, data.step);
    if (variable_octave < data.settings_rocket.pitch_range)
    {
        octave += variable_octave % 3 + 1;
    }

    uint8_t p_pitch = apply_scale(note_nr + harmony, data.scale, octave);

    if (cv(rocket.probs, data.step) <= (uint8_t)MIN(rocket.gate_density, 127))
    {
        uint8_t length = 32;
        if (!gate(rocket.slides, data.step, data.ticks))
        {
            all_notes_off(data.settings_rocket.storage, MIDI_CHANNEL_ROCKET);
            length = 6;
        }
        note_on(p_pitch, velocity, MIDI_CHANNEL_ROCKET, data.settings_rocket.storage, length);
    }
}
