#pragma once

#include "defs.h"

enum ScaleType {
    IONIAN,
    DORIAN,
    AEOLIAN
};

const uint8_t ionian[] = { 0, 2, 4, 5, 7, 9, 11 }; // 1 2  3 4 5 6   7
const uint8_t dorian[] = { 0, 2, 3, 5, 7, 9, 10 }; // 1 2 b3 4 5 6  b7
const uint8_t aeolian[] = { 0, 2, 3, 5, 7, 8, 10 }; // 1 2 b3 4 5 b6 b7 Natural Minor

void set_scale(Scale& scale, const ScaleType type)
{
    const uint8_t* notes_ptr;
    switch (type)
    {
    case ScaleType::IONIAN:
        scale.length = sizeof(ionian) / sizeof(uint8_t);
        notes_ptr = ionian;
    case ScaleType::DORIAN:
        scale.length = sizeof(dorian) / sizeof(uint8_t);
        notes_ptr = dorian;
    default:
    case ScaleType::AEOLIAN:
        scale.length = sizeof(aeolian) / sizeof(uint8_t);
        notes_ptr = aeolian;
    }

    for (int i = 0; i < scale.length; i++)
    {
        scale.notes[i] = notes_ptr[i];
    }
}

const Scale get_scale(const ScaleType scale_type, const Root root = ROOT_C)
{
    Scale scale = { 0 };

    scale.root = root;
    set_scale(scale, scale_type);

    return scale;
}

uint8_t apply_scale(uint8_t note_nr, Scale scale, uint8_t octave)
{
    uint8_t note = scale.notes[note_nr % scale.length]; // Actual pitch
    note += scale.root; // Transpose to correct root note
    note += octave * 12; // Transpose octaves
    return note;
}

uint8_t apply_scale_offset(uint8_t note_nr, Scale scale, uint8_t offset, uint8_t chord_step)
{
    uint8_t octave = note_nr / scale.length;
    uint8_t note = scale.notes[(note_nr + chord_step) % scale.length]; // Actual pitch
    note += scale.root; // Transpose to correct root note
    note = clip_pitch(note, offset, 127);
    note += octave * 12; // Transpose octaves
    return note;
}
