#pragma once

#include "defs.h"
#include "timing_structs.h"

class TimeStruct
{
public:
    uint32_t tick;
    PlayState state;
    uint32_t last_pulse_time;
    float average_pulse_time;
    uint8_t global_shuffle;

    TimeStruct()
    {
        tick = 0;
        state = PlayState::Stopped;
        last_pulse_time = 0;
        average_pulse_time = 500.f;
        global_shuffle = 0;
    }

    uint32_t step() const
    {
        return this->tick / TICKS_PER_STEP;
    }

    uint32_t get_shuffle_delay(
        const int8_t shuffle_offset = 0,
        const uint8_t global_delay = 0) const
    {
        uint32_t delay = (uint32_t)((this->average_pulse_time / 40.) * (global_delay / 127.));
        if ((this->tick / TICKS_PER_STEP) % 2 == 1)
        {
            uint8_t amount = (uint8_t)CLIP((int8_t)this->global_shuffle + shuffle_offset, 0, 127);
            return (uint32_t)((this->average_pulse_time / 8.) * (amount / 127.)) + delay;
        }
        else
            return delay;
    }

    uint32_t get_shuffle_delay(const MicroTimingStruct& micro) const
    {
        return this->get_shuffle_delay(micro.shuffle_off, micro.delay);
    }

    uint32_t get_count(const TimeDivision time_division) const
    {
        return this->tick / (uint32_t)time_division;
    }

};

