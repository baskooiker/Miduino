#pragma once

uint8_t ticks_left_in_bar(const TimeStruct& time)
{
    uint32_t tick_in_local_bar = time.tick % TICKS_IN_BAR;
    return TICKS_IN_BAR - tick_in_local_bar;
}

void reset_time(TimeStruct& time)
{
    time.tick = 0;
    time.state = PlayState::Stopped;
    time.last_pulse_time = 0;
    time.average_pulse_time = 500.f;
}
