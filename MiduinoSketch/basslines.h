
#ifndef BASSLINES_H
#define BASSLINES_H

#include "defs.h"
#include "rhythms.h"

Bassline init_bassline()
{
    Bassline b;
    b.pitches.length = 16;
    b.accents = init_gate_pattern_64();
    b.gates = init_gate_pattern_64();
    b.slides = init_gate_pattern_64();
    return b;
}

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

ChordPattern init_chord_pattern()
{
    ChordPattern chords;
    chords.gates = init_percussive_pattern_64();
    chords.pitches.length = 64;
    fill_bar(&chords.pitches, 0, 36);
    fill_bar(&chords.pitches, 1, 36);
    fill_bar(&chords.pitches, 2, 43);
    fill_bar(&chords.pitches, 3, 41);
    return chords;
}

uint8_t next_in_scale(uint8_t pitch, Root root, Scale scale, uint8_t step)
{
    uint8_t return_value = 0;
    return return_value;
}

uint8_t pitch(CvPattern64& pattern, long step)
{
    uint8_t s = (uint8_t)(step % pattern.length);
    return pattern.pattern[s];
}

uint8_t pitch(CvPattern16* pattern, long step)
{
    uint8_t s = uint8_t(step % pattern->length);
    return pattern->pattern[s];
}

uint8_t pitch(CvPatternAB& pattern, Scale scale, uint8_t octave, long step)
{
    uint8_t s = uint8_t(step % 64);
    uint8_t part = pattern.abPattern[s / 16];
    uint8_t note = pattern.patterns[part][s];
    note = scale.notes[note % scale.length]; // Actual pitch
    note += scale.root; // Transpose to correct root note
    note += octave * 12; // Transpose octaves
    return note;
}

CvPatternAB init_cv_pattern_ab()
{   
    CvPatternAB pattern = {0};
    return pattern;
}

#endif // BASSLINES_H

