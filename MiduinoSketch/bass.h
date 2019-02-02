#pragma once

#include "defs.h"
#include "harmony.h"
#include "midi_io.h"
#include "pitch.h"
#include "scales.h"
#include "rand.h"

void randomize_bass(ApplicationData& data)
{
    CvPatternAB& p_pattern = data.bass_settings.pitches;

    randomize(data.bass_settings.octaves, 2, randi(4, 6));
    randomize(data.bass_settings.pitches, data.harmony.scale.length);
    set_gates_low(data.bass_settings.low_pattern, 1);
    set_euclid(data.bass_settings.euclid_pattern, 16, 5);
    
    randomize(data.bass_settings.slides, .15f);

    randomize(data.bass_settings.note_range_prob);

    randomize_interval(data.bass_settings.int_pattern, arp_interval_probs);

    randomize(data.bass_settings.accents, .5f);
    randomize(data.bass_settings.variable_octaves);
}

bool get_bass_hit(BassSettings& settings, const uint32_t step, const uint8_t tick)
{
    bool hit = false;
    switch (settings.style)
    {
    case BassStyle::BassLow:
        hit = gate(settings.low_pattern, step, tick);
        break;
    case BassStyle::BassEuclid:
        hit = gate(settings.euclid_pattern, step, tick);
        break;
    case BassStyle::BassArpInterval:
        hit = interval_hit(settings.int_pattern, step, tick);
        break;
    case BassStyle::BassSixteenths:
        hit = interval_hit(TimeDivision::Sixteenth, step, tick);
        break;
    default: 
        hit = false;
    }
    return hit;
}

uint8_t get_bass_pitch(const BassSettings& settings, const HarmonyStruct& harmony, const uint32_t step, const uint8_t tick)
{
    uint8_t note_nr = 0;
    uint8_t note_range_p = cv(settings.note_range_prob, step);
    if (note_range_p < settings.note_range_value)
    {
        if (settings.note_range_value < 64)
        {
            note_nr = to_chord_order(cv(settings.pitches, step));
        }
        else
        {
            if (note_range_p % 64 < settings.note_range_value % 64)
            {
                note_nr = cv(settings.pitches, step);
            }
            else
            {
                note_nr = to_chord_order(cv(settings.pitches, step));
            }
        }
    }

    uint8_t octave = cv(settings.octaves, step);
    /*uint8_t variable_octave = cv(settings.variable_octaves, step);
    if (variable_octave < settings.pitch_range)
    {
        octave += variable_octave % 3 + 1;
    }*/
    octave += get_distributed_range(cv(settings.variable_octaves, step), settings.pitch_range, 3);

    uint8_t harmony_step = get_chord_step(harmony, step, tick);
    uint8_t pitch = apply_scale(note_nr + harmony_step, harmony.scale, octave);
    return pitch;
}

void play_bass(ApplicationData& data)
{
    const uint32_t step = data.step;
    const uint32_t tick = data.ticks;

    // Velocity
    uint8_t velocity = data.bass_settings.low_velocity;

    // Get hit
    bool hit = get_bass_hit(data.bass_settings, step, tick);

    hit &= !data.ui_state.kill_bass;

    if (hit)
    {
        uint8_t pitch = get_bass_pitch(data.bass_settings, data.harmony, step, tick);

        // Note length
        uint8_t length = 5;
        if (gate(data.bass_settings.slides, step, tick) 
            || data.bass_settings.style == BassStyle::BassLow)
        {
            length = ticks_left_in_bar(step, tick) - 6;
        }
        else
        {
            length = 5;
        }

        // Play it!
        note_on(make_note(pitch, velocity, length), data.bass_settings.storage);
    }
}
