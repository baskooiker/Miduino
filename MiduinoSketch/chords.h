#pragma once

#include "cv.h"
#include "rhythms.h"
#include "scale.h"
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

void get_chord(uint8_t chord, Scale& scale, uint8_t offset, uint8_t* chord_notes, uint8_t& size)
{
    size = 0;
    for (int i = offset; i < offset + 12; i++)
    {
        if (scale.chord_contains(i, chord))
        {
            chord_notes[size++] = i;
        }
    }
}
