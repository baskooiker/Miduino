#pragma once

#include "arp.h"
#include "chords.h"
#include "defs.h"
#include "euclid.h"
#include "harmony.h"
#include "midi_io.h"
#include "poly_settings.h"

void randomize_poly(PolySettings& settings)
{    
    // Set pattern low
    settings.gates_low.set_gates_low();

    // Set pattern high
    uint8_t steps = randui8(5, 11);
    set_euclid(settings.gates, 16, steps);
    set_ab_pattern(settings.gates.abPattern);

    // Set Tie Pattern
    settings.tie_pattern.randomize(randf(.1f, .4f));

    // Randomize pitch range
    settings.pitch_offset = randui8(42, 54);
}

void play_poly(PolySettings& settings, HarmonyStruct& harmony, const TimeStruct& time)
{ 
    uint8_t velocity = 64;

    bool hit = false;
    switch (settings.type)
    {
    case PolyType::PolyLow: hit = settings.gates_low.gate(time); break;
    case PolyType::PolyHigh: hit = settings.gates.gate(time); break;
    }

    if (hit)
    {
        const uint8_t MAX_CHORD_NOTES = 8;

        uint8_t chord_nr = harmony.get_chord_step(time);
        uint8_t size = 0;
        uint8_t chord_notes[MAX_CHORD_NOTES];

        // TODO: get poly pitch offset into settings 
        uint8_t poly_pitch_offset = 64;

        uint8_t pitch_offset = settings.pitch_offset 
            + (((uint16_t)poly_pitch_offset * 24) / 128) 
            - 12;
        get_chord(chord_nr, harmony.scale, pitch_offset, chord_notes, size);

        uint8_t length = 6;
        if (settings.tie_pattern.gate(time)
            || settings.type == PolyType::PolyLow)
        {
            length = time.ticks_left_in_bar();
        }

        NoteStruct note_structs[MAX_CHORD_NOTES] = { 0 };
        for (int i = 0; i < size; i++)
        {
            note_structs[i].pitch = chord_notes[i];
            note_structs[i].velocity = 64;
            note_structs[i].type = NoteType::Tie;
            note_structs[i].length = length;
        }

        settings.storage.note_on(note_structs, size);
    }
}
