#pragma once

#include <time.h>
#include <stdlib.h>

#include "defs.h"
#include "time_struct.h"
#include "rand.h"

void swap(uint8_t* array, uint8_t x, uint8_t y)
{
    uint8_t mem = array[x];
    array[x] = array[y];
    array[y] = mem;
}

void remove(const uint8_t item, uint8_t* array, uint8_t& length)
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
        swap(array, i, randui8(length));
    }
}

void find_item(const uint8_t item, const uint8_t* array, const uint8_t length, uint8_t& idx)
{
    for (uint8_t i = 0; i < length; i++)
    {
        if (item == array[i])
        {
            idx = i;
            return;
        }
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
        swap(array, i, randui8(length));
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

bool is_in_set(const uint8_t pitch, const uint8_t* set, const uint8_t size)
{
    for (int i = 0; i < size; i++)
    {
        if (pitch == set[i])
            return true;
    }
    return false;
}

uint32_t get_count(const TimeDivision time_division, const TimeStruct& time)
{
    return time.tick / (uint32_t)time_division;
}

uint8_t rerange(uint8_t input, uint8_t range, int8_t offset)
{
    return (uint8_t)(MIN(MAX(((((int16_t)input) * range) / 128 + offset), 0), 127));
}

bool gate(const uint16_t value, const uint8_t index, const uint8_t length = 16)
{
    return (value >> (index % length)) & 0x1;
}
