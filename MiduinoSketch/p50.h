#ifndef P50_H
#define P50_H

#include "defs.h"
#include "midi_io.h"

void play_P50(ApplicationData* data)
{ 
    ChordPattern* pattern = &data->p50_pattern;
    uint8_t velocity = 32;
    if (gate(pattern->gates, data->step))
    {
        uint8_t p = 0;
        do {
            p = pop_from_storage(data->storage_p50);
            if (p > 0)
            {
                note_off(p, MIDI_CHANNEL_P50, data->storage_p50);
            }
        } while (p != 0);

        uint8_t root = pitch(&pattern->pitches, data->step % 64);
        uint8_t fifth = root + 7;
        note_on(root , velocity, MIDI_CHANNEL_P50, data->storage_p50);
        note_on(fifth , velocity, MIDI_CHANNEL_P50, data->storage_p50);
    }
}
#endif // P50_H
