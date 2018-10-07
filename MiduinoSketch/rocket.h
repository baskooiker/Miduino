#pragma once

#include "defs.h"
#include "midi_io.h"
#include "scales.h"

void root_rocket_seq(ApplicationData& data)
{
    data.settings_rocket.follow_harmony = randomi(2);
    CvPatternAB& p_pattern = data.settings_rocket.pitches;
    fill_bar(p_pattern.patterns[0], 0);
    fill_bar(p_pattern.patterns[1], 0);
    fill_bar(p_pattern.patterns[2], 0);
    fill_bar(data.settings_rocket.octaves.patterns[0], 0);
    fill_bar(data.settings_rocket.octaves.patterns[1], 0);
    fill_bar(data.settings_rocket.octaves.patterns[2], 0);

    randomize_ab(data.settings_rocket.gates, data.settings_rocket.density);
    set_random_pattern_ab(data.settings_rocket.slides, .25f);
    data.settings_rocket.accents = init_percussive_pattern_64();
}

void modify_rocket_seq(ApplicationData& data)
{
    data.settings_rocket.follow_harmony = randomi(2);
    CvPatternAB& p_pattern = data.settings_rocket.pitches;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < NOTES_IN_BAR; j++)
        {
            if (randomf() < .25)
            {
                p_pattern.patterns[i][j] = data.scale.notes[data.scale.length];
            }
            if (randomf() < .25)
            {
                data.settings_rocket.octaves.patterns[i][j] = data.scale.notes[data.scale.length];
            }
        }
    }
    randomize_ab(data.settings_rocket.gates, data.settings_rocket.density);
    set_random_pattern_ab(data.settings_rocket.slides, .25f);
    data.settings_rocket.accents = init_percussive_pattern_64();
}

void randomize_octaves(SignedCvPattern& pattern)
{
    for (uint8_t i = 0; i < NOTES_IN_BAR; i++)
    {
        pattern[i] = ((randomi(3) - 1) * 12);
    }
}

void randomize_notes(CvPattern& pattern, uint8_t range)
{
    for (uint8_t i = 0; i < NOTES_IN_BAR; i++)
    {
        pattern[i] = randomi(range);
    }
}

void randomize_rocket_seq(ApplicationData& data)
{
    CvPatternAB& p_pattern = data.settings_rocket.pitches;

    randomize_octaves(data.settings_rocket.octaves.patterns[0]);
    randomize_octaves(data.settings_rocket.octaves.patterns[1]);
    randomize_octaves(data.settings_rocket.octaves.patterns[2]);

    randomize_notes(data.settings_rocket.pitches.patterns[0], data.scale.length);
    randomize_notes(data.settings_rocket.pitches.patterns[1], data.scale.length);
    randomize_notes(data.settings_rocket.pitches.patterns[2], data.scale.length);

    randomize_ab(data.settings_rocket.gates, data.settings_rocket.density);

    set_random_pattern_ab(data.settings_rocket.slides, .25f);

    data.settings_rocket.accents = init_percussive_pattern_64();
}

void play_rocket(ApplicationData& data)
{
    SettingsRocket& rocket = data.settings_rocket;
  
    uint8_t velocity = rocket.low_velocity;
    if (gate(rocket.accents, data.step))
    {
        velocity = rocket.high_velocity;
    }

    uint8_t p = pitch(rocket.pitches, data.step);
    uint8_t harmony = pitch(data.settings_p50.pattern.pitches, data.step);
    uint8_t octave = get_octave(rocket.octaves, data.step);
    uint8_t note_nr = apply_scale(p + harmony, data.scale, octave);

    if (gate(rocket.gates, data.step))
    {
        if (!gate(rocket.slides, data.step))
        {
            all_notes_off(data.settings_rocket.storage, MIDI_CHANNEL_ROCKET);
        }
        note_on(p, velocity, MIDI_CHANNEL_ROCKET, data.settings_rocket.storage);
    }
}
