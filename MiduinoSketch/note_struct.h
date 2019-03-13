#pragma once

#include "enums.h"

class NoteStruct
{
public:
    uint8_t pitch;
    uint8_t velocity;
    uint8_t length;
    NoteType type;
};

class NoteEvent
{
public:
    NoteStruct note;
    uint32_t time;
};
