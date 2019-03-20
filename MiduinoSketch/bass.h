#pragma once

#include "defs.h"
#include "fugue.h"
#include "harmony.h"
#include "midi_io.h"
#include "pitch.h"
#include "rand.h"
#include "utils.h"

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
        hit = settings.low_pattern.gate(time);
        break;
    case BassStyle::BassEuclid:
        hit = settings.euclid_pattern.gate(time);
        break;
    case BassStyle::BassArpInterval:
        hit = interval_hit(settings.int_pattern, time);
        break;
    }

    uint8_t prob = settings.probs.value(time);
    bool prob_step = (prob < density) && (prob > 0) && interval_hit(TimeDivision::Sixteenth, time);
    return hit || prob_step;
}

uint8_t get_bass_pitch(
    const BassSettings& settings, 
    const HarmonyStruct& harmony, 
    const TimeStruct& time,
    const uint8_t variable_pitch,
    const uint8_t note_offset)
{
    // TODO: Hier klopt dus niks van...
    uint8_t note_nr = 0;
    uint8_t note_range_p = settings.note_range_prob.value(time);

    // TODO: Deze geeft veel te vaak false.
    if (note_range_p < settings.note_range_value)
    {
        uint8_t pitch_cv = harmony.scale.get_note(settings.pitches.value(time));

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

    uint8_t pitch = harmony.scale.apply_scale_offset(
        note_nr, 
        pitch_offset, 
        harmony.get_chord_step(time) + note_offset
    );

    uint8_t octave = settings.octaves.value(time);
    if (variable_pitch < settings.pitch_range)
    {
        pitch += (variable_pitch % 3 + 1) * 12;
    }
    
    pitch = clip_pitch(pitch, pitch_offset, rerange(variable_pitch, 36, pitch_offset + 12));

    pitch += settings.octave_offset * 12;
    return pitch;
}

void play_bass(
    BassSettings& settings, 
    const HarmonyStruct& harmony, 
    FugueSettings& fugue,
    const TimeStruct& time)
{
    if (settings.style == BassStyle::BassFugue && (time.tick % TICKS_PER_STEP) == 0)
    {
        return play_fugue(
            fugue,
            settings.fugue_id, 
            harmony, 
            time, 
            settings.storage);
    }

    // Get hit
    bool hit = get_bass_hit(settings, settings.density, time);

    if (hit)
    {
        uint8_t pitch = get_bass_pitch(
            settings, 
            harmony, 
            time, 
            settings.variable_octaves.value(time),
            NoteInterval::IntervalRoot
        );

        // Note length
        uint8_t length = settings.accents.gate(time) ? 6 : 2;
        if (settings.slides.gate(time))
        {
            length = ticks_left_in_bar(time);
        }

        // Play it!
        settings.storage.note_on(
            make_note(pitch, 64, length, NoteType::Tie), 
            time.get_shuffle_delay()
        );
    }
}
