#pragma once

#include "arp.h"
#include "chords.h"
#include "defs.h"
#include "euclid.h"
#include "midi_io.h"
#include "rhythm_time.h"

void randomize_P50_seq(ApplicationData& data)
{    
    // Set pattern low
    set_gates_low(data.settings_p50.gates_low, randi(1, 4));

    // Set pattern high
    uint8_t steps = randi(5, 11);
    set_euclid(data.settings_p50.gates, 16, steps);
    set_ab_pattern(data.settings_p50.gates.abPattern);

    // Set Tie Pattern
    randomize(data.settings_p50.tie_pattern, randf(.1f, .4f));

    // Randomize pitch range
    data.settings_p50.pitch_offset = randi(42, 54);
}

void play_P50(ApplicationData& data)
{ 
    uint8_t velocity = 64;

    bool hit = false;
    switch (data.settings_p50.type)
    {
    case PolyType::PolyLow: hit = gate(data.settings_p50.gates_low, data.step, data.ticks); break;
    case PolyType::PolyHigh: hit = gate(data.settings_p50.gates, data.step, data.ticks); break;
    }

    if (hit)
    {
        const uint8_t MAX_CHORD_NOTES = 8;

        uint8_t chord_nr = cv(data.harmony, data.step);
        uint8_t size = 0;
        uint8_t chord_notes[MAX_CHORD_NOTES];

        get_chord(chord_nr, data.scale, data.settings_p50.pitch_offset, chord_notes, size);

        uint8_t length = 6;
        if (gate(data.settings_p50.tie_pattern, data.step, data.ticks) 
            || data.settings_p50.type == PolyType::PolyLow)
        {
            length = ticks_left_in_bar(data.step, data.ticks);
        }

        NoteStruct note_structs[MAX_CHORD_NOTES] = { 0 };
        for (int i = 0; i < size; i++)
        {
            note_structs[i].pitch = chord_notes[i];
            note_structs[i].velocity = 64;
            note_structs[i].holding = false;
            note_structs[i].length = length;
        }

        note_on(note_structs, size, MIDI_CHANNEL_P50, data.settings_p50.storage);
    }
}
