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
    CvPatternAB& p_pattern = settings.pitches;

    randomize(settings.octaves, 2, randi(4, 6));
    randomize(settings.pitches);
    set_gates_low(settings.low_pattern, 1);
    set_euclid(settings.euclid_pattern, 16, 5);
    
    randomize(settings.slides, .15f);

    randomize(settings.note_range_prob);

    randomize_interval(settings.int_pattern, arp_interval_probs);

    randomize(settings.accents, .5f);
    randomize(settings.variable_octaves);
}

bool get_bass_hit(BassSettings& settings, const TimeStruct& time)
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
    case BassStyle::BassSixteenths:
        hit = interval_hit(TimeDivision::Sixteenth, time);
        break;
    }
    return hit;
}

uint8_t get_bass_pitch(const BassSettings& settings, const HarmonyStruct& harmony, const TimeStruct& time)
{
    // TODO: Hier klopt dus niks van...
    uint8_t note_nr = 0;
    uint8_t note_range_p = cv(settings.note_range_prob, time.step);

    // TODO: Deze geeft veel te vaak false.
    if (note_range_p < settings.note_range_value)
    {
        uint8_t pitch_cv = cv(settings.pitches, time.step) % harmony.scale.length;

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

    uint8_t octave = cv(settings.octaves, time.step);
    /*uint8_t variable_octave = cv(settings.variable_octaves, step);
    if (variable_octave < settings.pitch_range)
    {
        octave += variable_octave % 3 + 1;
    }*/
    octave += get_distributed_range(cv(settings.variable_octaves, time.step), settings.pitch_range, 3);

    uint8_t harmony_step = get_chord_step(harmony, time);
    uint8_t pitch = apply_scale(note_nr + harmony_step, harmony.scale, octave);

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
    bool hit = get_bass_hit(data.bass_settings, time);

    if (hit)
    {
        uint8_t pitch = get_bass_pitch(data.bass_settings, data.harmony, time);

        // Note length
        uint8_t length = 5;

        // Play it!
        note_on(make_note(pitch, 64, length, NoteType::Tie), data.bass_settings.storage);
    }
}
