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
    // Randomize octaves
    randomize(settings.octaves, 2, randi(4, 6));
    switch (distribution(16, 16, 16, 32))
    {
    case 0: settings.octaves.length = 2; break;
    case 1: settings.octaves.length = 4; break;
    case 2: settings.octaves.length = 8; break;
    case 3: settings.octaves.length = 16; break;
    }
    randomize(settings.variable_octaves);
    switch (distribution(16, 16, 16, 32))
    {
    case 0: settings.variable_octaves.length = 2; break;
    case 1: settings.variable_octaves.length = 4; break;
    case 2: settings.variable_octaves.length = 8; break;
    case 3: settings.variable_octaves.length = 16; break;
    }

    // Randomize pitches
    randomize(settings.pitches);
    switch (distribution(16, 16, 16, 32))
    {
    case 0: settings.pitches.length = 2; break;
    case 1: settings.pitches.length = 4; break;
    case 2: settings.pitches.length = 8; break;
    case 3: settings.pitches.length = 16; break;
    }
    randomize(settings.note_range_prob);
    switch (distribution(16, 16, 16, 32))
    {
    case 0: settings.note_range_prob.length = 2; break;
    case 1: settings.note_range_prob.length = 4; break;
    case 2: settings.note_range_prob.length = 8; break;
    case 3: settings.note_range_prob.length = 16; break;
    }

    // Randomize gates
    set_gates_low(settings.low_pattern, 1);
    randomize(settings.probs);
    switch (distribution(16, 16, 32))
    {
    case 0: settings.note_range_prob.length = 4; break;
    case 1: settings.note_range_prob.length = 8; break;
    case 2: settings.note_range_prob.length = 16; break;
    }

    // Randomize style
    switch (distribution(10, 10))
    {
    case 0: settings.style = BassStyle::BassArpInterval; break;
    case 1: settings.style = BassStyle::BassEuclid; break;
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
        set_euclid(settings.euclid_pattern, 16, steps);
        settings.euclid_pattern.length = 16;
    }
    else if (step_dist == 1)
    {
        switch (distribution(40, 20))
        {
        case 0: steps = 3;  break;
        case 1: steps = 4;  break;
        }
        set_euclid(settings.euclid_pattern, 8, steps);
        settings.euclid_pattern.length = 8;
    }
    
    // Randomize others
    randomize_interval(settings.int_pattern, arp_interval_probs);
    randomize(settings.slides, .15f);
    randomize(settings.accents, randf(.15f, 1.f));
    settings.note_range_value = quad(randi()) / 2;
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
        uint8_t length = gate(data.bass_settings.accents, time) ? 6 : 2;

        // Play it!
        note_on(
            make_note(pitch, 64, length, NoteType::Tie), 
            data.bass_settings.storage, 
            get_shuffle_delay(time)
        );
    }
}
