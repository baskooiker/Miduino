#pragma once

#include "core/defs.h"
#include "core/enums.h"

namespace Vleerhond
{
    class NoteStruct
    {
    public:
        uint8_t pitch;
        uint8_t velocity;
        uint8_t length;
        NoteType type;

        NoteStruct();

        NoteStruct(const uint8_t pitch, const uint8_t velocity, const uint8_t length = 1, const NoteType type = NoteType::Tie);
    };

    class NoteEvent
    {
    public:
        NoteStruct note;
        uint32_t time;

        NoteEvent();

        NoteEvent(const NoteStruct note, const uint32_t time);
    };
}