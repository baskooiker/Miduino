#pragma once

#include "cv.h"

void set_chord_pattern_ab(CvPatternAB& pattern)
{
    // TODO
    uint8_t options[] = {2, 3, 4, 5};
    randomize_order(options, 4);
    set_all(pattern.patterns[0], 0);
    set_all(pattern.patterns[1], options[0]);
    set_all(pattern.patterns[2], options[1]);
    set_ab_pattern(pattern.abPattern);
}

bool is_in_chord(const uint8_t pitch, const Scale& scale, const uint8_t chord)
{
    // TODO
    uint8_t note = (pitch + 12 - scale.root) % 12;
    for (int i = 0; i < scale.length; i++)
    {
        if (note == scale.notes[i])
            return true;
    }
    return false;
}

