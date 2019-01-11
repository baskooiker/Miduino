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

const Scale get_scale(const ScaleType scale_type, const Root root = ROOT_C)
{
    Scale scale;
    const uint8_t* notes_ptr;
    switch(scale_type)
    {
        case IONIAN:
            scale.length = sizeof(ionian) / sizeof(uint8_t);
            notes_ptr = ionian;
        case DORIAN:
            scale.length = sizeof(dorian) / sizeof(uint8_t);
            notes_ptr = dorian;
        default:
        case AEOLIAN:
            scale.length = sizeof(aeolian) / sizeof(uint8_t);
            notes_ptr = aeolian;
    }

    for (int i = 0; i < scale.length; i++)
    {
        scale.notes[i] = notes_ptr[i];
    }
    scale.root = root;
    return scale;
}
