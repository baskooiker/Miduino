#pragma once

#include "defs.h"

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
};

