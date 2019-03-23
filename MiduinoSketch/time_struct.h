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
protected:
    uint8_t global_shuffle;

public:
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
        if ((this->tick + TICKS_PER_STEP) % (2 * TICKS_PER_STEP) == 0)
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

    void randomize_shuffle()
    {
        const int8_t rand_range = 32;
        int8_t min_ = MAX((int8_t)this->global_shuffle - rand_range, 0);
        int8_t max_ = MIN((int8_t)this->global_shuffle + rand_range, 128);
        this->global_shuffle = Rand::randui8(min_, max_);
    }

    uint8_t ticks_left_in_bar() const
    {
        uint32_t tick_in_local_bar = this->tick % TICKS_IN_BAR;
        return TICKS_IN_BAR - tick_in_local_bar;
    }

    void reset()
    {
        this->tick = 0;
        this->state = PlayState::Stopped;
        this->last_pulse_time = 0;
        this->average_pulse_time = 500.f;
    }

};

