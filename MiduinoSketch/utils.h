#pragma once

#include <time.h>
#include <stdlib.h>

#include "defs.h"
#include "rand.h"

void swap(uint8_t* array, uint8_t x, uint8_t y)
{
    uint8_t mem = array[x];
    array[x] = array[y];
    array[y] = mem;
}

void randomize_order(uint8_t* array, uint8_t length)
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
