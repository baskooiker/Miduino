#pragma once

#include "chords.h"
#include "defs.h"
#include "rand.h"

uint8_t get_closest(
    const ArpData& arp_data, 
    const uint8_t* arp_pitches, 
    const uint8_t size, 
    const bool include_current, 
    uint8_t& last_note)
{
    last_note = CLIP(arp_data.last_note, arp_data.min, arp_data.min + arp_data.range);
    if (is_in_set(arp_data.last_note, arp_pitches, size) && include_current)
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
                last_note -= c;
                return last_note;
            }
        }
        else
        {
            below = true;
        }

        if (last_note + c < arp_data.min + arp_data.range)
        {
            if (is_in_set(last_note + c, arp_pitches, size))
            {
                last_note += c;
                return last_note;
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

uint8_t get_arp_pitch(
    const ArpData& arp_data, 
    uint8_t& arp_counter,
    uint8_t &last_note)
{
    if (arp_data.arp_notes_length == 0)
        return 0;

    switch (arp_data.type)
    {
    case UP: 
        arp_counter = arp_counter % arp_data.arp_notes_length;
        return arp_data.arp_notes[arp_counter++];
    case DOWN:
        arp_counter = arp_data.counter % arp_data.arp_notes_length;
        return arp_data.arp_notes[arp_data.arp_notes_length - ++arp_counter];
    case UPDOWN:
        arp_counter = arp_counter % (2 * arp_data.arp_notes_length - 2);
        if (arp_data.counter < arp_data.arp_notes_length)
            return arp_data.arp_notes[arp_counter++];
        else
            return arp_data.arp_notes[arp_data.arp_notes_length - (arp_counter++ - arp_data.arp_notes_length + 2)];
    case PICKING_IN:
        arp_counter = arp_counter % arp_data.arp_notes_length;
        if (arp_data.counter % 2 == 0)
            return arp_data.arp_notes[arp_counter++ / 2];
        else
            return arp_data.arp_notes[arp_data.arp_notes_length - ++arp_counter / 2];
    case CLOSEST:
        return get_closest(arp_data, arp_data.arp_notes, arp_data.arp_notes_length, true, last_note);
    case CLOSEST_EXC:
        return get_closest(arp_data, arp_data.arp_notes, arp_data.arp_notes_length, false, last_note);
    case RANDOM:
        return arp_data.arp_notes[randui8(arp_data.arp_notes_length)];
    }
    return 0;
}

void get_arp_pitches_by_range(ArpData& arp_data, const Scale& scale, const uint8_t chord)
{
    arp_data.arp_notes_length = 0;
    for (int i = arp_data.min; i < arp_data.min + arp_data.range; i++)
    {
        if (scale.chord_contains(i, chord))
            arp_data.arp_notes[arp_data.arp_notes_length++] = i;
    }
}

void get_arp_pitches_by_count(ArpData& arp_data, const Scale& scale, const uint8_t chord)
{
    int i = arp_data.min;
    arp_data.arp_notes_length = 0;
    while (arp_data.arp_notes_length < arp_data.range_count
        && i < 128)
    {
        if (scale.chord_contains(i, chord))
            arp_data.arp_notes[arp_data.arp_notes_length++] = i;
        i++;
    }
}

uint8_t get_next_arp_pitch(ArpData& arp_data, const Scale& scale, const uint8_t chord)
{
    switch (arp_data.range_type)
    {
    case RangeType::Range: get_arp_pitches_by_range(arp_data, scale, chord); break;
    case RangeType::Count: get_arp_pitches_by_count(arp_data, scale, chord); break;
    }

    return get_arp_pitch(arp_data, arp_data.counter, arp_data.last_note);
}

uint8_t get_arp_pitch(const ArpData& arp_data)
{
    uint8_t counter = arp_data.counter;
    uint8_t last_note = arp_data.last_note;
    return get_arp_pitch(arp_data, counter, last_note);
}
