#ifndef ROCKET_H
#define ROCKET_H

#include "defs.h"
#include "midi_io.h"
#include "scales.h"

void root_rocket_seq(ApplicationData* data)
{
    CvPattern16* p_pattern = &data->rocket_pattern.pitches;
    for (int i = 0; i < p_pattern->length; i++)
    {
        p_pattern->pattern[i] = (data->rocket_octave * 12) + data->root;
    }
    randomize_ab(&data->rocket_pattern.gates, data->rocket_density);
    randomize_ab(&data->rocket_pattern.slides, .25f);
    data->rocket_pattern.accents = init_percussive_pattern_64();
}

void modify_rocket_seq(ApplicationData& data)
{
    CvPattern16* p_pattern = &data.rocket_pattern.pitches;
    for (int i = 0; i < p_pattern->length; i++)
    {
        if ((random(1024) / 1024.) < .25)
        {
            p_pattern->pattern[i] = data.scale.notes[data.scale.length] + ((random(3) - 1) * 12) + (data.rocket_octave * 12) + data.root;
        }
    }
    randomize_ab(&data.rocket_pattern.gates, data.rocket_density);
    randomize_ab(&data.rocket_pattern.slides, .25f);
    data.rocket_pattern.accents = init_percussive_pattern_64();
}

void randomize_rocket_seq(ApplicationData* data)
{
    CvPattern16* p_pattern = &data->rocket_pattern.pitches;
    for (int i = 0; i < p_pattern->length; i++)
    {
        p_pattern->pattern[i] = data->scale.notes[data->scale.length] + ((random(3) - 1) * 12) + (data->rocket_octave * 12) + data->root;
    }
    randomize_ab(&data->rocket_pattern.gates, data->rocket_density);
    randomize_ab(&data->rocket_pattern.slides, .25f);
    data->rocket_pattern.accents = init_percussive_pattern_64();
}

void play_rocket(ApplicationData* data)
{
    Bassline* rocket = &data->rocket_pattern;
  
    uint8_t p = pitch(&rocket->pitches, data->step);
    uint8_t velocity = 32;
    if (gate(rocket->accents, data->step))
    {
        velocity = 100;
    }
  
    if (gate(rocket->gates, data->step))
    {
        if (!gate(rocket->slides, data->step))
        {
            all_notes_off(data->storage_rocket, MIDI_CHANNEL_ROCKET);
        }
        note_on(p, velocity, MIDI_CHANNEL_ROCKET, data->storage_rocket);
    }
}

#endif // ROCKET_H
