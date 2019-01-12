#pragma once

#include "chords.h"
#include "defs.h"
#include "rand.h"

uint8_t get_arp_pitch(ArpData& arp_data, const uint8_t* arp_pitches, const uint8_t size)
{
    if (size == 0)
        return 0;

    switch (arp_data.type)
    {
    case UP: 
        arp_data.counter = arp_data.counter % size;
        return arp_pitches[arp_data.counter++];
    case DOWN:
        arp_data.counter = arp_data.counter % size;
        return arp_pitches[size - ++arp_data.counter];
    case UPDOWN:
        arp_data.counter = arp_data.counter % (2 * size - 2);
        if (arp_data.counter < size)
            return arp_pitches[arp_data.counter++];
        else
            return arp_pitches[size - (arp_data.counter++ - size + 2)];
    case PICKING_IN:
        arp_data.counter = arp_data.counter % size;
        if (arp_data.counter % 2 == 0)
            return arp_pitches[arp_data.counter++ / 2];
        else
            return arp_pitches[size - ++arp_data.counter / 2];
    case CLOSEST:
    {
        arp_data.last_note = CLIP(arp_data.last_note, arp_data.min, arp_data.min+arp_data.range);
        if (is_in_set(arp_data.last_note, arp_pitches, size))
            return arp_data.last_note;
        bool below = false;
        bool above = false;
        uint8_t c = 1;
        while (!below || !above)
        {
            if (arp_data.last_note - c > arp_data.min)
            {
                if (is_in_set(arp_data.last_note - c, arp_pitches, size))
                {
                    arp_data.last_note -= c;
                    return arp_data.last_note;
                }
            }
            else
            {
                below = true;
            }

            if (arp_data.last_note + c < arp_data.min + arp_data.range)
            {
                if (is_in_set(arp_data.last_note + c, arp_pitches, size))
                {
                    arp_data.last_note += c;
                    return arp_data.last_note;
                }
            }
            else
            {
                above = true;
            }
            c++;
        }
        return 0;
    }
    case RANDOM:
        return arp_pitches[randi(size)];
    }
    return 0;
}

uint8_t get_arp_pitch(ArpData& arp_data, const Scale scale, const uint8_t chord)
{
    uint8_t arp_pitches[32] = {0};
    uint8_t size = 0;
    for (int i = arp_data.min; i < arp_data.min + arp_data.range; i++)
    {
        if (is_in_chord(i, scale, chord))
            arp_pitches[size++] = i;
    }

    return get_arp_pitch(arp_data, arp_pitches, size);
}

