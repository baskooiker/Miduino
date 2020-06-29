#include "utils/utils.h"

#include "utils/rand.h"

namespace Vleerhond
{
    namespace Utils
    {
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

        void randomizeOrder(uint8_t* array, uint8_t length)
        {
            for (uint8_t i = 0; i < length; i++)
            {
                swap(array, i, Rand::randui8(length));
            }
        }

        void findItem(const uint8_t item, const uint8_t* array, const uint8_t length, uint8_t& idx)
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

        NoteInterval randomNoteInterval()
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

        void swap(NoteInterval* array, uint8_t x, uint8_t y)
        {
            NoteInterval mem = array[x];
            array[x] = array[y];
            array[y] = mem;
        }

        void randomizeOrder(NoteInterval* array, uint8_t length)
        {
            for (uint8_t i = 0; i < length; i++)
            {
                swap(array, i, Rand::randui8(length));
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

        bool isInSet(const uint8_t pitch, const std::vector<uint8_t> set)
        {
            for (const uint8_t item : set)
            {
                if (pitch == item)
                    return true;
            }
            return false;
        }

        uint8_t rerange(uint8_t input, uint8_t range, int8_t offset)
        {
            return (uint8_t)(MIN(MAX(((((int16_t)input) * range) / 128 + offset), 0), 127));
        }

        bool gate(const uint16_t value, const uint8_t index, const uint8_t length)
        {
            return (value >> (index % length)) & 0x1;
        }

        uint8_t toChordOrder(const uint8_t order)
        {
            if (order < 2)
                return 0;
            if (order < 4)
                return 2;
            if (order < 6)
                return 4;
            return 0;
        }

        uint8_t clipPitch(const uint8_t pitch, const uint8_t minimum, uint8_t maximum)
        {
            maximum = MAX(minimum + 12, maximum);
            if (pitch < minimum)
                return clipPitch(pitch + 12, minimum, maximum);
            if (pitch >= maximum)
                return clipPitch(pitch - 12, minimum, maximum);
            return pitch;
        }

        uint8_t clipPitch(const uint8_t pitch, const uint8_t min)
        {
            return clipPitch(pitch, min, min + 12);
        }

        uint8_t quad(const uint8_t v)
        {
            double d = (double)(v / 127.);
            return (uint8_t)(127.9 * d * d);
        }

        uint8_t logu8(const uint8_t v)
        {
            return (uint8_t)(127.9 * log(((double)v / 127.)));
        }

        bool intervalHit(const TimeDivision time_division, const TimeStruct& time)
        {
            return time.tick % (uint32_t)time_division == 0;
        }

        uint32_t millis()
        {
            return ofGetSystemTimeMillis();
        }
    };
}