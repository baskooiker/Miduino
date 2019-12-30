#pragma once

#include <time.h>
#include <stdlib.h>

#include "core/defs.h"
#include "core/time_struct.h"
#include "rand.h"

namespace Vleerhond
{
    class Utils
    {
    public:
        static void swap(uint8_t* array, uint8_t x, uint8_t y)
        {
            uint8_t mem = array[x];
            array[x] = array[y];
            array[y] = mem;
        }

        static void remove(const uint8_t item, uint8_t* array, uint8_t& length)
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

        static void randomize_order(uint8_t* array, uint8_t length)
        {
            for (uint8_t i = 0; i < length; i++)
            {
                swap(array, i, Rand::randui8(length));
            }
        }

        static void find_item(const uint8_t item, const uint8_t* array, const uint8_t length, uint8_t& idx)
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

        static NoteInterval random_note_interval()
        {
            switch (Rand::distribution(16, 16, 16))
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

        static void swap(NoteInterval* array, uint8_t x, uint8_t y)
        {
            NoteInterval mem = array[x];
            array[x] = array[y];
            array[y] = mem;
        }

        static void randomize_order(NoteInterval* array, uint8_t length)
        {
            for (uint8_t i = 0; i < length; i++)
            {
                swap(array, i, Rand::randui8(length));
            }
        }

        static void sort(uint8_t* ar, uint8_t length)
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

        static bool is_in_set(const uint8_t pitch, const uint8_t* set, const uint8_t size)
        {
            for (int i = 0; i < size; i++)
            {
                if (pitch == set[i])
                    return true;
            }
            return false;
        }

        static uint8_t rerange(uint8_t input, uint8_t range, int8_t offset = 0)
        {
            return (uint8_t)(MIN(MAX(((((int16_t)input) * range) / 128 + offset), 0), 127));
        }

        static bool gate(const uint16_t value, const uint8_t index, const uint8_t length = 16)
        {
            return (value >> (index % length)) & 0x1;
        }

        static uint8_t to_chord_order(const uint8_t order)
        {
            if (order < 2)
                return 0;
            if (order < 4)
                return 2;
            if (order < 6)
                return 4;
            return 0;
        }

        static uint8_t clip_pitch(const uint8_t pitch, const uint8_t minimum, uint8_t maximum)
        {
            maximum = MAX(minimum + 12, maximum);
            if (pitch < minimum)
                return clip_pitch(pitch + 12, minimum, maximum);
            if (pitch >= maximum)
                return clip_pitch(pitch - 12, minimum, maximum);
            return pitch;
        }

        static uint8_t clip_pitch(const uint8_t pitch, const uint8_t min)
        {
            return clip_pitch(pitch, min, min + 12);
        }

        static uint8_t quad(const uint8_t v)
        {
            double d = (double)(v / 127.);
            return (uint8_t)(127.9 * d * d);
        }

        static uint8_t logu8(const uint8_t v)
        {
            return (uint8_t)(127.9 * log(((double)v / 127.)));
        }

        static bool interval_hit(const TimeDivision time_division, const TimeStruct& time)
        {
            return time.tick % (uint32_t)time_division == 0;
        }

    };
}