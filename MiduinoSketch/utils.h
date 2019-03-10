#pragma once

#include <time.h>
#include <stdlib.h>

#include "defs.h"
#include "note_handling.h"
#include "rand.h"

void swap(uint8_t* array, uint8_t x, uint8_t y)
{
    uint8_t mem = array[x];
    array[x] = array[y];
    array[y] = mem;
}

void remove(uint8_t item, uint8_t* array, uint8_t& length)
{
    for (int i = 0; i < length; i++)
    {
        if (array[i] == item)
        {
            swap(array, i, length - 1);
            length--;
        }
    }
}

void randomize_order(uint8_t* array, uint8_t length)
{
    for (uint8_t i = 0; i < length; i++)
    {
        swap(array, i, randi(length));
    }
}

NoteInterval random_note_interval()
{
    switch (distribution(16, 16, 16))
    {
    default:
    case 0:
        return NoteInterval::IntervalFifth;
    case 1:
        return NoteInterval::IntervalThird;
    case 2:
        return NoteInterval::IntervalFifth;
    }
}

void swap(NoteInterval* array, uint8_t x, uint8_t y)
{
    NoteInterval mem = array[x];
    array[x] = array[y];
    array[y] = mem;
}

void randomize_order(NoteInterval* array, uint8_t length)
{
    for (uint8_t i = 0; i < length; i++)
    {
        swap(array, i, randi(length));
    }
}

void sort(uint8_t* ar, uint8_t length)
{
    for (uint8_t i = 0; i < length; i++)                     //Loop for descending ordering
    {
        for (int j = 0; j < length; j++)             //Loop for comparing other values
        {
            if (ar[j] > ar[i])                //Comparing other array elements
            {
                uint8_t tmp = ar[i];         //Using temporary variable for storing last value
                ar[i] = ar[j];            //replacing value
                ar[j] = tmp;             //storing last value
            }
        }
    }
}

NoteStruct make_note(const uint8_t pitch, const uint8_t velocity, const uint8_t length = 1, const NoteType type = NoteType::Tie)
{
    NoteStruct n = { 0 };
    n.pitch = pitch;
    n.velocity = velocity;
    n.length = length;
    n.type = type;
    return n;
}

void all_notes_off(PitchStorage& storage)
{
    NoteStruct p = { 0, 0, 0 };
    do {
        p = pop_from_storage(storage);
        if (p.pitch > 0)
        {
            note_off(p.pitch, storage);
        }
    } while (p.pitch != 0);
}
