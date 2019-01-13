#pragma once

#include "arp.h"
#include "basslines.h"
#include "defs.h"
#include "euclid.h"
#include "midi_io.h"
#include "chords.h"

void randomize_P50_seq(ApplicationData& data)
{    
    // Set pattern high
    data.settings_p50.gates = init_gate_pattern_ab();
    uint8_t steps = randi(5, 11);
    set_euclid(data.settings_p50.gates, 16, steps);
    set_ab_pattern(data.settings_p50.gates.abPattern);

    // Set pattern low
    steps = randi(3, 4);
    uint8_t length = steps == 4 ? 15 : 16;

    if (randi(4) < 2)
    {
        data.settings_p50.gates_low.patterns[1] = 0x00;
        set_euclid(data.settings_p50.gates_low.patterns[0], length, steps);
    }
    else
    {
        data.settings_p50.gates_low.patterns[0] = 0x00;
        set_euclid(data.settings_p50.gates_low.patterns[1], length, steps);
    }

    data.settings_p50.gates_low.patterns[2] = 0x00;
    set_ab_pattern_high(data.settings_p50.gates_low.abPattern);
    data.settings_p50.gates_low.length = 16;
}

void play_P50(ApplicationData& data)
{ 
    if (data.ticks % TICKS_PER_STEP != 0)
    {
        return;
    }

    CvPatternAB& pattern = data.harmony;
    uint8_t velocity = 64;

    if (data.step % 16 == 0)
    {
        all_notes_off(data.settings_p50.storage, MIDI_CHANNEL_P50);
    }

    bool hit = false;
    switch (data.settings_p50.type)
    {
    case PolyType::PolyOff: break;
    case PolyType::PolyLow: hit = gate(data.settings_p50.gates_low, data.step, data.ticks);  break;
    case PolyType::PolyHigh: hit = gate(data.settings_p50.gates, data.step, data.ticks); break;
    }

    uint8_t note_nr = cv(pattern, data.step);
    if (hit)
    {
        all_notes_off(data.settings_p50.storage, MIDI_CHANNEL_P50);

        NoteStruct pitches[3] = { 0 };
        pitches[0].pitch = apply_scale(note_nr, data.scale, data.settings_p50.octave);
        pitches[1].pitch = apply_scale(note_nr + 2, data.scale, data.settings_p50.octave);
        pitches[2].pitch = apply_scale(note_nr + 4, data.scale, data.settings_p50.octave);

        note_on(pitches[0].pitch, velocity, MIDI_CHANNEL_P50, data.settings_p50.storage, 6);
        note_on(pitches[1].pitch, velocity, MIDI_CHANNEL_P50, data.settings_p50.storage, 6);
        note_on(pitches[2].pitch, velocity, MIDI_CHANNEL_P50, data.settings_p50.storage, 6);
    }
}
