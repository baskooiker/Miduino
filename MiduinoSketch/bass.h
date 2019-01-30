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

    randomize(data.bass_settings.octaves, 2, 4);
    randomize(data.bass_settings.pitches, data.scale.length);
    set_gates_low(data.bass_settings.low_pattern, 1);
    set_euclid(data.bass_settings.euclid_pattern, 16, 5);
    
    randomize(data.bass_settings.slides, .15f);

    randomize(data.bass_settings.note_range_prob);

    randomize_interval(data.bass_settings.int_pattern, arp_interval_probs);

    randomize(data.bass_settings.accents, .5f);
    randomize(data.bass_settings.variable_octaves);
}

void play_bass(ApplicationData& data)
{
    // Velocity
    uint8_t velocity = data.bass_settings.low_velocity;

    // Get hit
    bool hit = false;
    switch (data.bass_settings.style)
    {
    case RocketStyle::RocketLow:
        hit = gate(data.bass_settings.low_pattern, data.step, data.ticks);
        break;
    case RocketStyle::RocketEuclid:
        hit = gate(data.bass_settings.euclid_pattern, data.step, data.ticks);
        break;
    case RocketStyle::RocketArpInterval:
        hit = interval_hit(data.bass_settings.int_pattern, data.step, data.ticks);
        break;
    case RocketStyle::RocketSixteenths:
        hit = interval_hit(TimeDivision::Sixteenth, data.step, data.ticks); 
        break;
    default: break;
    }

    hit &= !data.ui_state.kill_bass;

    if (hit)
    {
        // Pitch
        uint8_t note_nr = 0;
        uint8_t note_range_p = cv(data.bass_settings.note_range_prob, data.step);
        if (note_range_p < data.bass_settings.note_range_value)
        {
            if (data.bass_settings.note_range_value < 64)
            {
                note_nr = to_chord_order(cv(data.bass_settings.pitches, data.step));
            }
            else
            {
                if (note_range_p % 64 < data.bass_settings.note_range_value % 64)
                {
                    note_nr = cv(data.bass_settings.pitches, data.step);
                }
                else
                {
                    note_nr = to_chord_order(cv(data.bass_settings.pitches, data.step));
                }
            }
        }

        uint8_t harmony = get_chord_step(data.harmony, data.scale, data.step, data.ticks);
        uint8_t octave = cv(data.bass_settings.octaves, data.step);
        uint8_t variable_octave = cv(data.bass_settings.variable_octaves, data.step);
        if (variable_octave < data.bass_settings.pitch_range)
        {
            octave += variable_octave % 3 + 1;
        }

        uint8_t pitch = apply_scale(note_nr + harmony, data.scale, octave);

        // Note length
        uint8_t length = 5;
        if (gate(data.bass_settings.slides, data.step, data.ticks) 
            || data.bass_settings.style == RocketStyle::RocketLow)
        {
            length = TICKS_IN_BAR;
        }
        else
        {
            //all_notes_off(data.bass_settings.storage, MIDI_CHANNEL_BASS);
            length = 5;
        }

        // Play it!
        note_on(make_note(pitch, velocity, length), data.bass_settings.storage);
    }
}
