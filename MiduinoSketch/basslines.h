
#ifndef BASSLINES_H
#define BASSLINES_H

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

void fill_bar(SignedCvPattern& pattern, const int8_t value)
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
    return chords;
}

uint8_t pitch(CvPattern64& pattern, long step)
{
    uint8_t s = (uint8_t)(step % pattern.length);
    return pattern.pattern[s];
}

uint8_t pitch(const CvPattern16& pattern, long step)
{
    uint8_t s = uint8_t(step % pattern.length);
    return pattern.pattern[s];
}

uint8_t pitch(CvPatternAB& pattern, long step)
{
    uint8_t s = uint8_t(step % 64);
    uint8_t part = pattern.abPattern[s / 16];
    uint8_t note = pattern.patterns[part][s % 16];
    return note;
}

uint8_t get_octave(SignedCvPatternAB& pattern, long step)
{
    uint8_t s = uint8_t(step % 64);
    uint8_t part = pattern.abPattern[s / 16];
    uint8_t octave = pattern.patterns[part][s % 16];
    return octave;
}

uint8_t apply_scale(uint8_t note_nr, Scale scale, uint8_t octave = 0)
{
    uint8_t note = scale.notes[note_nr % scale.length]; // Actual pitch
    note += scale.root; // Transpose to correct root note
    note += octave * 12; // Transpose octaves
    return note;
}

#endif // BASSLINES_H

