#ifndef SCALES_H
#define SCALES_H

#include "defs.h"

enum ScaleType {
	IONIAN,
	DORIAN,
	AEOLIAN
};

typedef struct {
    uint8_t notes[8];
    uint8_t length;
} Scale;

const uint8_t ionian[] = { 0, 2, 4, 5, 7, 9, 11 }; // 1 2  3 4 5 6   7
const uint8_t dorian[] = { 0, 2, 3, 5, 7, 9, 10 }; // 1 2 b3 4 5 6  b7
const uint8_t aeolian[] = { 0, 2, 3, 5, 7, 8, 10 }; // 1 2 b3 4 5 b6 b7 Natural Minor

const Scale get_scale(const ScaleType scale_type)
{
    Scale scale;
    switch(scale_type)
    {
        case IONIAN:
            scale.length = sizeof(ionian) / sizeof(uint8_t);
            for (int i = 0; i < scale.length; i++)
            {
                scale.notes[i] = ionian[i];
            }
        case DORIAN:
            scale.length = sizeof(dorian) / sizeof(uint8_t);
            for (int i = 0; i < scale.length; i++)
            {
                scale.notes[i] = dorian[i];
            }
        default:
        case AEOLIAN:
            scale.length = sizeof(aeolian) / sizeof(uint8_t);
            for (int i = 0; i < scale.length; i++)
            {
                scale.notes[i] = aeolian[i];
            }
    }
    return scale;
}

#endif // SCALES_H
