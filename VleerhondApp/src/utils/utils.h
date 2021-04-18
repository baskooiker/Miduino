#pragma once

#include <time.h>
#include <stdlib.h>

#include "ofMain.h"

#include "core/defs.h"
#include "core/time_struct.h"

namespace Vleerhond
{
    namespace Utils
    {
        void swap(uint8_t* array, uint8_t x, uint8_t y);
        void remove(const uint8_t item, uint8_t* array, uint8_t& length);
        void randomizeOrder(uint8_t* array, uint8_t length);
        void findItem(const uint8_t item, const uint8_t* array, const uint8_t length, uint8_t& idx);
        NoteInterval randomNoteInterval();
        void swap(NoteInterval* array, uint8_t x, uint8_t y);
        void randomizeOrder(NoteInterval* array, uint8_t length);
        void sort(uint8_t* ar, uint8_t length);
        bool isInSet(const uint8_t pitch, const std::vector<uint8_t> set);
        uint8_t rerange(uint8_t input, uint8_t range, int8_t offset = 0);
        bool gate(const uint16_t value, const uint8_t index, const uint8_t length = 16);
        uint8_t toChordOrder(const uint8_t order);
        uint8_t clipPitch(const uint8_t pitch, const uint8_t minimum, uint8_t maximum);
        uint8_t clipPitch(const uint8_t pitch, const uint8_t min);
        uint8_t quad(const uint8_t v);
        uint8_t logu8(const uint8_t v);
        bool intervalHit(const TimeDivision time_division, const TimeStruct& time);
        uint32_t millis();
    };
}