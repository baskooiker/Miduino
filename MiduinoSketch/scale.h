#pragma once

#include "utils.h"

const uint8_t ionian[] = { 0, 2, 4, 5, 7, 9, 11 }; // 1 2  3 4 5 6   7
const uint8_t ionian_penta[] = { 0, 2, 4,    7, 9 }; // 1 2  3 4 5 6   7

const uint8_t dorian[] = { 0, 2, 3, 5, 7, 9, 10 }; // 1 2 b3 4 5 6  b7
const uint8_t dorian_penta[] = { 0,    3, 5, 7,    10 }; // 1 2 b3 4 5 6  b7

const uint8_t aeolian[] = { 0, 2, 3, 5, 7, 8, 10 }; // 1 2 b3 4 5 b6 b7 Natural Minor
const uint8_t aeolian_penta[] = { 0,    3, 5, 7,    10 }; // 1 2 b3 4 5 b6 b7 Natural Minor

class Scale
{
public:
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
        this->type = type_;
    }

    uint8_t* get_scale_notes(uint8_t& length) const
    {
        length = 7;
        switch (this->type)
        {
        default:
        case ScaleType::AEOLIAN: return (uint8_t*)aeolian;
        case ScaleType::DORIAN: return (uint8_t*)dorian;
        case ScaleType::IONIAN: return (uint8_t*)ionian;
        }
    }

    uint8_t apply_scale(uint8_t note_nr, uint8_t octave)
    {
        uint8_t length = 0;
        uint8_t* notes = get_scale_notes(length);
        uint8_t note = notes[note_nr % length]; // Actual pitch
        note += root; // Transpose to correct root note
        note += octave * 12; // Transpose octaves
        return note;
    }

    uint8_t apply_scale_offset(uint8_t note_nr, uint8_t offset, uint8_t chord_step) const
    {
        uint8_t length = 0;
        uint8_t* notes = get_scale_notes(length);
        uint8_t octave = note_nr / length;
        uint8_t note = notes[(note_nr + chord_step) % length]; // Actual pitch
        note += root; // Transpose to correct root note
        note = Utils::clip_pitch(note, offset, 127);
        note += octave * 12; // Transpose octaves
        return note;
    }

    bool contains(const uint8_t pitch) const
    {
        uint8_t length = 0;
        uint8_t* notes = get_scale_notes(length);
        return Utils::is_in_set((pitch + 12 - this->root) % 12, notes, length);
    }

    bool chord_contains(const uint8_t pitch, const uint8_t chord) const 
    {
        uint8_t length = 0;
        uint8_t* notes = get_scale_notes(length);
        for (int i = 0; i < 5; i += 2)
        {
            uint8_t note = ((pitch + 12 - this->root) % 12);
            uint8_t chord_note = notes[(i + chord) % length];
            if (note == chord_note)
                return true;
        }
        return false;
    }

    uint8_t get_note(const uint8_t v) const
    {
        uint8_t length = 0;
        uint8_t* notes = get_scale_notes(length);
        return notes[v % length];
    }

    void get_available_chords_indices(uint8_t* chords, uint8_t& length) const
    {
        length = 0;
        uint8_t scale_length = 0;
        uint8_t* notes = this->get_scale_notes(scale_length);
        for (int i = 0; i < scale_length; i++)
        {
            if (this->contains(notes[i] + 7))
            {
                chords[length++] = i;
            }
        }
    }

};
