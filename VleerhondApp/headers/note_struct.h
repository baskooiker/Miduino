#pragma once

#include "enums.h"

namespace Vleerhond
{
    class NoteStruct
    {
    public:
        uint8_t pitch;
        uint8_t velocity;
        uint8_t length;
        NoteType type;

        NoteStruct()
        {
            pitch = 0;
            velocity = 0;
            length = 0;
            type = NoteType::Normal;
        }

        NoteStruct(const uint8_t pitch, const uint8_t velocity, const uint8_t length = 1, const NoteType type = NoteType::Tie)
        {
            this->pitch = pitch;
            this->velocity = velocity;
            this->length = length;
            this->type = type;
        }
    };

    class NoteEvent
    {
    public:
        NoteStruct note;
        uint32_t time;

        NoteEvent()
        {
            time = 0;
        }

        NoteEvent(const NoteStruct note, const uint32_t time) :
            note(note),
            time(time)
        {
        }
    };
}