#pragma once

#include <iostream>

#include "core/defs.h"
#include "midi/midi_io.h"

static uint16_t note_hits[12] = { 0 };

class PrintMidi
{
public:
    static void reset_note_hits()
    {
        for (int i = 0; i < 12; i++)
        {
            note_hits[i] = 0;
        }
    }
    
    static void print_note_hits()
    {
        for (int i = 0; i < 12; i++)
        {
            printf("Note %-2d: %-3d\n", i, note_hits[i]);
        }
        printf("\n");
    }
};
