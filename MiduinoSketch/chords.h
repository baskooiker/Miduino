#pragma once

#include "cv.h"
#include "rhythms.h"
#include "utils.h"

void set_chord_pattern_ab(CvPatternAB& pattern)
{
    uint8_t options[] = {2, 3, 4, 5};
    randomize_order(options, 4);
    set_all(pattern.patterns[0], 0);
    set_all(pattern.patterns[1], options[0]);
    set_all(pattern.patterns[2], options[1]);
    set_ab_pattern(pattern.abPattern);
}

bool is_in_set(const uint8_t pitch, const uint8_t* set, const uint8_t size)
{
    for (int i = 0; i < size; i++)
    {
        if (pitch == set[i])
            return true;
    }
    return false;
}

bool is_in_scale(const uint8_t pitch, const Scale& scale)
{
    return is_in_set((pitch + 12 - scale.root) % 12, scale.notes, scale.length);
}

bool is_in_chord(const uint8_t pitch, const Scale& scale, const uint8_t chord)
{
    for (int i = 0; i < 5; i+=2)
    {
        uint8_t note = ((pitch + 12 - scale.root) % 12);
        uint8_t chord_note = scale.notes[(i + chord) % scale.length];
        if (note == chord_note)
            return true;
    }
    return false;
}

void get_chord(uint8_t chord, Scale& scale, uint8_t offset, uint8_t* chord_notes, uint8_t& size)
{
    size = 0;
    for (int i = offset; i < offset + 12; i++)
    {
        if (is_in_chord(i, scale, chord))
        {
            chord_notes[size++] = i;
        }
    }
}
