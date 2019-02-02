#pragma once

#include "arp.h"
#include "chords.h"
#include "defs.h"
#include "euclid.h"
#include "harmony.h"
#include "midi_io.h"
#include "rhythm_time.h"

void randomize_poly(ApplicationData& data)
{    
    // Set pattern low
    set_gates_low(data.poly_settings.gates_low, randi(1, 4));

    // Set pattern high
    uint8_t steps = randi(5, 11);
    set_euclid(data.poly_settings.gates, 16, steps);
    set_ab_pattern(data.poly_settings.gates.abPattern);

    // Set Tie Pattern
    randomize(data.poly_settings.tie_pattern, randf(.1f, .4f));

    // Randomize pitch range
    data.poly_settings.pitch_offset = randi(42, 54);
}

void play_poly(ApplicationData& data, const TimeStruct& time)
{ 
    uint8_t velocity = 64;

    bool hit = false;
    switch (data.poly_settings.type)
    {
    case PolyType::PolyLow: hit = gate(data.poly_settings.gates_low, time.step, time.tick); break;
    case PolyType::PolyHigh: hit = gate(data.poly_settings.gates, time.step, time.tick); break;
    }

    if (hit)
    {
        const uint8_t MAX_CHORD_NOTES = 8;

        uint8_t chord_nr = get_chord_step(data.harmony, time.step, time.tick);
        uint8_t size = 0;
        uint8_t chord_notes[MAX_CHORD_NOTES];

        uint8_t pitch_offset = data.poly_settings.pitch_offset 
            + (((uint16_t)data.ui_state.poly_pitch_offset * 24) / 128) 
            - 12;
        get_chord(chord_nr, data.harmony.scale, pitch_offset, chord_notes, size);

        uint8_t length = 6;
        if (gate(data.poly_settings.tie_pattern, time.step, time.tick) 
            || data.poly_settings.type == PolyType::PolyLow)
        {
            length = ticks_left_in_bar(time.step, time.tick);
        }

        NoteStruct note_structs[MAX_CHORD_NOTES] = { 0 };
        for (int i = 0; i < size; i++)
        {
            note_structs[i].pitch = chord_notes[i];
            note_structs[i].velocity = 64;
            note_structs[i].type = NoteType::Tie;
            note_structs[i].length = length;
        }

        note_on(note_structs, size, data.poly_settings.storage);
    }
}
