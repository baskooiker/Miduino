#pragma once

#include "defs.h"
#include "midi_io.h"
#include "chords.h"

void randomize_P50_seq(ApplicationData& data)
{
    set_chord_pattern_ab(data.settings_p50.pattern);
}

void play_P50(ApplicationData& data)
{ 
    ChordPatternAB& pattern = data.settings_p50.pattern;
    uint8_t velocity = 32;
    if (gate(pattern.gates, data.step))
    {
        all_notes_off(data.storage_p50, MIDI_CHANNEL_P50);
        
        uint8_t note_nr = pitch(pattern.pitches, data.step);
        uint8_t root = apply_scale(note_nr, data.scale, data.settings_p50.octave);
        uint8_t third = apply_scale(note_nr + 2, data.scale, data.settings_p50.octave);
        uint8_t fifth = apply_scale(note_nr + 4, data.scale, data.settings_p50.octave);
        
        note_on(root, velocity, MIDI_CHANNEL_P50, data.storage_p50);
        note_on(third, velocity, MIDI_CHANNEL_P50, data.storage_p50);
        note_on(fifth , velocity, MIDI_CHANNEL_P50, data.storage_p50);
    }
}
