#pragma once

#include "arp.h"
#include "chords.h"
#include "defs.h"
#include "euclid.h"
#include "midi_io.h"
#include "rhythm_time.h"

void randomize_P50_seq(ApplicationData& data)
{    
    // Set pattern high
    uint8_t steps = randi(5, 11);
    set_euclid(data.settings_p50.gates, 16, steps);
    set_ab_pattern(data.settings_p50.gates.abPattern);

    // Set pattern low
    if (randi(2) < 1)
        steps = 3;
    else
        steps = 5;

    if (randi(4) < 2)
    {
        data.settings_p50.gates_low.patterns[1] = 0x00;
        set_euclid(data.settings_p50.gates_low.patterns[0], 16, steps);
    }
    else
    {
        data.settings_p50.gates_low.patterns[0] = 0x00;
        set_euclid(data.settings_p50.gates_low.patterns[1], 16, steps);
    }

    data.settings_p50.gates_low.patterns[2] = 0x00;
    set_ab_pattern_high(data.settings_p50.gates_low.abPattern);
    data.settings_p50.gates_low.length = 16;

    // Set Tie Pattern
    randomize(data.settings_p50.tie_pattern, randf(.1f, .4f));
}

void play_P50(ApplicationData& data)
{ 
    if (data.ticks % TICKS_PER_STEP != 0)
    {
        return;
    }

    uint8_t velocity = 64;

    if (data.step % 16 == 0)
    {
        all_notes_off(data.settings_p50.storage, MIDI_CHANNEL_P50);
    }

    bool hit = false;
    switch (data.settings_p50.type)
    {
    /*case PolyType::PolyOff: break;
    case PolyType::PolyLow: hit = gate(data.settings_p50.gates_low, data.step, data.ticks);  break;*/
    case PolyType::PolyHigh: hit = gate(data.settings_p50.gates, data.step, data.ticks); break;
    }

    if (hit)
    {
        const uint8_t MAX_CHORD_NOTES = 8;

        //all_notes_off(data.settings_p50.storage, MIDI_CHANNEL_P50);

        uint8_t chord_nr = cv(data.harmony, data.step);
        uint8_t size = 0;
        uint8_t chord_notes[MAX_CHORD_NOTES];

        // TODO: Make offset variable, parameterized, LFO'd
        uint8_t offset = 48;
        get_chord(chord_nr, data.scale, offset, chord_notes, size);

        NoteStruct note_structs[MAX_CHORD_NOTES] = { 0 };
        for (int i = 0; i < size; i++)
        {
            note_structs[i].pitch = chord_notes[i];
            note_structs[i].velocity = 64;
            note_structs[i].holding = false;
            note_structs[i].length = gate(data.settings_p50.tie_pattern, data.step, data.ticks) ? 
                ticks_left_in_bar(data.step, data.ticks) : 
                6;
        }

        note_on(note_structs, size, MIDI_CHANNEL_P50, data.settings_p50.storage);
    }
}
