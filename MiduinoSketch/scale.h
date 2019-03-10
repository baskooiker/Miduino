#pragma once

#include "scales.h"

const uint8_t ionian[] = { 0, 2, 4, 5, 7, 9, 11 }; // 1 2  3 4 5 6   7
const uint8_t ionian_penta[] = { 0, 2, 4,    7, 9 }; // 1 2  3 4 5 6   7

const uint8_t dorian[] = { 0, 2, 3, 5, 7, 9, 10 }; // 1 2 b3 4 5 6  b7
const uint8_t dorian_penta[] = { 0,    3, 5, 7,    10 }; // 1 2 b3 4 5 6  b7

const uint8_t aeolian[] = { 0, 2, 3, 5, 7, 8, 10 }; // 1 2 b3 4 5 b6 b7 Natural Minor
const uint8_t aeolian_penta[] = { 0,    3, 5, 7,    10 }; // 1 2 b3 4 5 b6 b7 Natural Minor

class Scale
{
public:
    uint8_t notes[8];
    uint8_t length;
    Root root;
    ScaleType type;

    Scale()
    {
        set_scale(ScaleType::AEOLIAN);
        root = Root::ROOT_C;
    }

    Scale(ScaleType type_, Root root_)
    {
        set_scale(type_);
        root = root_;
    }

    void set_scale(const ScaleType type_)
    {
        const uint8_t* notes_ptr;
        switch (type_)
        {
        case ScaleType::IONIAN:
            length = sizeof(ionian) / sizeof(uint8_t);
            notes_ptr = ionian;
            break;
        case ScaleType::DORIAN:
            length = sizeof(dorian) / sizeof(uint8_t);
            notes_ptr = dorian;
            break;
        default:
        case ScaleType::AEOLIAN:
            length = sizeof(aeolian) / sizeof(uint8_t);
            notes_ptr = aeolian;
            break;
        }

        for (int i = 0; i < length; i++)
        {
            notes[i] = notes_ptr[i];
        }
        type = type_;
    }

    uint8_t apply_scale(uint8_t note_nr, uint8_t octave)
    {
        uint8_t note = notes[note_nr % length]; // Actual pitch
        note += root; // Transpose to correct root note
        note += octave * 12; // Transpose octaves
        return note;
    }

    uint8_t apply_scale_offset(uint8_t note_nr, uint8_t offset, uint8_t chord_step)
    {
        uint8_t octave = note_nr / length;
        uint8_t note = notes[(note_nr + chord_step) % length]; // Actual pitch
        note += root; // Transpose to correct root note
        note = clip_pitch(note, offset, 127);
        note += octave * 12; // Transpose octaves
        return note;
    }

};
