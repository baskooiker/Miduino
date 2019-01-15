#pragma once

#include "defs.h"
#include "init.h"
#include "rhythms.h"

void fill_bar(CvPattern& pattern, const uint8_t value)
{
    for (uint8_t i = 0; i < 16; i++)
    {
        pattern[i] = value;
    }
}

void fill_bar(CvPattern64* pattern, const uint8_t bar_nr, const uint8_t value)
{
    for (uint8_t i = 0; i < 16; i++)
    {
        pattern->pattern[bar_nr * 16 + i] = value;
    }
}

uint8_t get_octave(CvPatternAB& pattern, long step)
{
    uint8_t s = uint8_t(step % 64);
    uint8_t part = pattern.abPattern[s / 16];
    uint8_t octave = pattern.patterns[part][s % 16];
    return octave;
}

uint8_t apply_scale(uint8_t note_nr, Scale scale, uint8_t octave)
{
    uint8_t note = scale.notes[note_nr % scale.length]; // Actual pitch
    note += scale.root; // Transpose to correct root note
    note += octave * 12; // Transpose octaves
    return note;
}
