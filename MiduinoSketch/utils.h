#pragma once

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "defs.h"
#include "rand.h"

bool gate(const BinaryPattern& gates, const long step)
{
    return (((gates) & (uint16_t)(0x1 << (uint16_t)(step % 16)))) > 0;
}

bool gate(const GatePattern16& pattern, const long step)
{
    return gate(pattern.pattern, step % pattern.length);
}

bool gate(const GatePattern64& pattern, const long step)
{
    uint8_t s = uint8_t(step % pattern.length);
    uint8_t sub_pat = s / 16;
    uint8_t sub_step = s % 16;
    return gate(pattern.patterns[sub_pat], sub_step);
}

bool gate(const GatePatternAB& pattern, const long step)
{
    uint8_t s = step % 64;
    uint8_t sub_pat = pattern.abPattern[s / 16];
    uint8_t sub_step = s % 16;
    return gate(pattern.patterns[sub_pat], sub_step);
}

void set_gate(BinaryPattern& gates, const uint8_t step, const bool val)
{
    if (val)
    {
        gates |= ((uint16_t)(0x1) << step);
    }
    else
    {
        gates &= ~((uint16_t)(0x1) << step);
    }
}

void print_pattern(const GatePatternAB& pattern)
{
    for (long bar = 0; bar < 4; bar++)
    {
        for (long step = 0; step < 16; step++)
        { 
            char sig = gate(pattern, bar * 16 + step) ? 'X' : '.';
            printf("%c ", sig);
        }
        printf("\n");
    }
}

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