#pragma once

#include "defs.h"
#include "fugue.h"
#include "harmony.h"
#include "midi_io.h"
#include "pitch.h"
#include "scales.h"
#include "rand.h"

void randomize_bass(BassSettings& settings)
{
    randomize(settings.octaves, 2, randi(4, 6));
    randomize(settings.pitches);
    set_gates_low(settings.low_pattern, 1);

    switch (distribution(10, 10))
    {
    case 0: settings.style = BassStyle::BassArpInterval; break;
    case 1: settings.style = BassStyle::BassEuclid; break;
    }

    uint8_t steps = 5;
    switch (distribution(20, 20))
    {
    case 0: steps = 3;  break;
    case 1: steps = 5;  break;
    }
    set_euclid(settings.euclid_pattern, 16, steps);
    
    randomize(settings.slides, .15f);
    randomize(settings.note_range_prob);
    randomize_interval(settings.int_pattern, arp_interval_probs);
    randomize(settings.accents, .5f);
    randomize(settings.variable_octaves);
    randomize(settings.probs);
    settings.note_range_value = quad(randi());
}

bool get_bass_hit(BassSettings& settings, const uint8_t density, const TimeStruct& time)
{
    if (settings.kill)
    {
        return false;
    }

    bool hit = false;
    switch (settings.style)
    {
    case BassStyle::BassLow:
        hit = gate(settings.low_pattern, time);
        break;
    case BassStyle::BassEuclid:
        hit = gate(settings.euclid_pattern, time);
        break;
    case BassStyle::BassArpInterval:
        hit = interval_hit(settings.int_pattern, time);
        break;
    }

    uint8_t prob = cv(settings.probs, time);
    bool prob_step = (prob < density) && (prob > 0) && interval_hit(TimeDivision::Sixteenth, time);
    return hit || prob_step;
}

uint8_t get_bass_pitch(const BassSettings& settings, const HarmonyStruct& harmony, const TimeStruct& time)
{
    // TODO: Hier klopt dus niks van...
    uint8_t note_nr = 0;
    uint8_t note_range_p = cv(settings.note_range_prob, time);

    // TODO: Deze geeft veel te vaak false.
    if (note_range_p < settings.note_range_value)
    {
        uint8_t pitch_cv = cv(settings.pitches, time) % harmony.scale.length;

        if (settings.note_range_value < 64)
        {
            note_nr = to_chord_order(pitch_cv);
        }
        else
        {
            if (note_range_p % 64 < settings.note_range_value % 64)
            {
                note_nr = pitch_cv;
            }
            else
            {
                note_nr = to_chord_order(pitch_cv);
            }
        }
    }

    const uint8_t pitch_offset = 36;

    uint8_t pitch = apply_scale_offset(
        note_nr, 
        harmony.scale, 
        pitch_offset, 
        get_chord_step(harmony, time)
    );

    uint8_t octave = cv(settings.octaves, time);
    uint8_t variable_octave = cv(settings.variable_octaves, time);
    if (variable_octave < settings.pitch_range)
    {
        pitch += (variable_octave % 3 + 1) * 12;
    }
    
    pitch = clip_pitch(pitch, pitch_offset, apply_cv(variable_octave, 36, pitch_offset + 12));

    pitch += settings.octave_offset * 12;
    return pitch;
}

void play_bass(ApplicationData& data, const TimeStruct& time)
{
    if (data.bass_settings.style == BassStyle::BassFugue && time.tick == 0)
    {
        return play_fugue(
            data.fugue_settings,
            data.bass_settings.fugue_id, 
            data.harmony, 
            time, 
            data.bass_settings.storage);
    }

    // Get hit
    bool hit = get_bass_hit(data.bass_settings, data.bass_settings.density, time);

    if (hit)
    {
        uint8_t pitch = get_bass_pitch(data.bass_settings, data.harmony, time);

        // Note length
        uint8_t length = 5;

        // Play it!
        note_on(make_note(pitch, 64, length, NoteType::Tie), data.bass_settings.storage);
    }
}
