#pragma once

#include "cv_patterns.h"
#include "rhythms.h"
#include "scale.h"
#include "utils.h"

void set_chord_pattern_ab(CvPatternAB& pattern)
{
    uint8_t options[] = {2, 3, 4, 5};
    randomize_order(options, 4);
    pattern.patterns[0].set_all(0);
    pattern.patterns[1].set_all(options[0]);
    pattern.patterns[2].set_all(options[1]);
    set_ab_pattern(pattern.abPattern);
}

const uint8_t chord_time_patterns[][4] = {
    {8,  8, 0, 0},
    {10, 6, 0, 0},
    {12, 4, 0, 0},
    {8,  4, 4, 0},
    {6,  6, 4, 0},
    {4,  4, 4, 4},
    {6,  2, 6, 2},
};
const uint8_t nr_of_chord_time_patterns = sizeof(chord_time_patterns) / sizeof(*chord_time_patterns);

void get_chord_time_pattern(uint8_t* pattern, uint8_t& length)
{
    uint8_t pat_idx = randui8(nr_of_chord_time_patterns);
    length = 0;

    for (int i = 0; i < 4; i++)
    {
        if (chord_time_patterns[pat_idx][i] > 0)
        {
            length++;
        }
        pattern[i] = chord_time_patterns[pat_idx][i];
    }
}

void get_chord_seq(uint8_t* options, uint8_t length, uint8_t* seq)
{
    if (length == 4)
    {
        uint8_t ab_pat[4] = { 0, 0, 0, 0 };
        set_ab_pattern_high(ab_pat);
        for (int i = 0; i < 4; i++)
        {
            uint8_t idx = ab_pat[i];
            if (idx == 0)
                seq[i] = 0;
            else
                seq[i] = options[idx - 1];
        }
    }
    else
    {
        seq[0] = 0;
        for (int i = 0; i < 3; i++)
        {
            seq[i + 1] = options[i];
        }
    }
}

void set_chord_pattern(CvPattern16& pattern)
{
    uint8_t options[] = { 2, 3, 4, 5 };
    randomize_order(options, 4);

    uint8_t length = 0;
    uint8_t chord_time_pattern[4] = { 0, 0, 0, 0 };
    get_chord_time_pattern(chord_time_pattern, length);

    uint8_t seq[4] = { 0, 0, 0, 0 };
    get_chord_seq(options, length, seq);

    uint8_t c = 0;
    for (int i = 0; i < length && i < 4; i++)
    {
        for (int j = 0; j < chord_time_pattern[i]; j++)
        {
            pattern.set(c, seq[i]);
            c += 1;
        }
    }
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
