#pragma once

#include "ofLog.h"

#include "core/defs.h"
#include "core/timing_structs.h"

namespace Vleerhond
{
    class TimeStruct
    {
    public:
        uint32_t tick;
        PlayState state;
        uint32_t last_pulse_time;
        float average_pulse_time;
        uint8_t global_shuffle;

    public:
        TimeStruct();
        uint32_t step() const;
        uint32_t get_shuffle_delay(
            const int8_t shuffle_offset = 0,
            const uint8_t global_delay = 0) const;
        uint32_t get_shuffle_delay(const MicroTimingStruct& micro) const;
        uint32_t get_count(const TimeDivision time_division) const;
        void randomize_shuffle();
        uint8_t ticks_left_in_bar() const;
        void reset();
        TimeStruct add(const int32_t offset);
    };
}