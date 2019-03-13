#pragma once

#include "defs.h"

#define STORAGE_SIZE 8

class PitchStorage
{
public:
    NoteStruct data[STORAGE_SIZE];
    uint8_t size;

    NoteEvent events[STORAGE_SIZE];
    uint8_t nr_of_events;

    uint8_t channel;

    PitchStorage()
    {
        size = 0;
        nr_of_events = 0;
    }
};
