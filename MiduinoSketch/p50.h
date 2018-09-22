#ifndef P50_H
#define P50_H

#include "defs.h"
#include "midi_io.h"

void randomize_P50_seq(ApplicationData* data)
{
    data->p50_pattern = init_chord_pattern();
}

void play_P50(ApplicationData* data)
{ 
    ChordPattern* pattern = &data->p50_pattern;
    uint8_t velocity = 32;
    if (gate(pattern->gates, data->step))
    {
        all_notes_off(data->storage_p50, MIDI_CHANNEL_P50);
        
        uint8_t root = pitch(&pattern->pitches, data->step % 64);
        uint8_t fifth = root + 7;
        note_on(root , velocity, MIDI_CHANNEL_P50, data->storage_p50);
        note_on(fifth , velocity, MIDI_CHANNEL_P50, data->storage_p50);
    }
}
#endif // P50_H
