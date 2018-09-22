#ifndef SCALES_H
#define SCALES_H

#include "defs.h"

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
