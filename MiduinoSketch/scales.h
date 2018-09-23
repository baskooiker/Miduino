#ifndef SCALES_H
#define SCALES_H

#include "defs.h"

enum Scale {
	IONIAN,
	DORIAN,
	AEOLIAN
};

const uint8_t ionian[] = { 0, 2, 4, 5, 7, 9, 11 }; // 1 2  3 4 5 6   7
const uint8_t dorian[] = { 0, 2, 3, 5, 7, 9, 10 }; // 1 2 b3 4 5 6  b7
const uint8_t aeolian[] = { 0, 2, 3, 5, 7, 8, 10 }; // 1 2 b3 4 5 b6 b7 Natural Minor

const uint8_t* get_scale(const Scale scale)
{
    switch(scale)
    {
        case IONIAN:
            return ionian;
        case DORIAN:
            return dorian;
        case AEOLIAN:
            return aeolian;
    }
    return 0;
}

#endif // SCALES_H
