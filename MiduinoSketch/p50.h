#pragma once

#include "defs.h"
#include "euclid.h"
#include "midi_io.h"
#include "chords.h"

void randomize_P50_seq(ApplicationData& data)
{    
    data.settings_p50.gates = init_gate_pattern_ab();
    uint8_t steps = randi(3, 11);
    set_euclid(data.settings_p50.gates.patterns[0], 16, steps);
    set_euclid(data.settings_p50.gates.patterns[1], 16, steps);
    set_euclid(data.settings_p50.gates.patterns[2], 16, steps);
    set_ab_pattern(data.settings_p50.gates.abPattern);
}

void play_P50(ApplicationData& data)
{ 
    ChordPatternAB& pattern = data.harmony;
    uint8_t velocity = 32;

    if (data.step % 16 == 0)
    {
        all_notes_off(data.settings_p50.storage, MIDI_CHANNEL_P50);
    }

    uint8_t note_nr = pitch(pattern.pitches, data.step);
    if (gate(data.settings_p50.gates, data.step))
    {    
        uint8_t root = apply_scale(note_nr, data.scale, data.settings_p50.octave);
        uint8_t third = apply_scale(note_nr + 2, data.scale, data.settings_p50.octave);
        uint8_t fifth = apply_scale(note_nr + 4, data.scale, data.settings_p50.octave);
        
        note_on(root, velocity, MIDI_CHANNEL_P50, data.settings_p50.storage);
        note_on(third, velocity, MIDI_CHANNEL_P50, data.settings_p50.storage);
        note_on(fifth , velocity, MIDI_CHANNEL_P50, data.settings_p50.storage);
    }

    if (data.settings_p50.play_arp)
    {
        static uint8_t arp_choices[] = { 0, 2, 4 };

        uint8_t pitch = apply_scale(note_nr + arp_choices[randi(3)], data.scale, data.settings_p50.octave + 2);
        note_on(pitch, velocity, MIDI_CHANNEL_P50, data.settings_p50.storage);
    }
}
