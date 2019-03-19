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

NoteStruct make_note(const uint8_t pitch, const uint8_t velocity, const uint8_t length = 1, const NoteType type = NoteType::Tie)
{
    NoteStruct n = { 0 };
    n.pitch = pitch;
    n.velocity = velocity;
    n.length = length;
    n.type = type;
    return n;
}
