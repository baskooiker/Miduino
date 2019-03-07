#pragma once

#define TICKS_IN_BAR 96

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

uint32_t get_shuffle_delay(
    const TimeStruct& time, 
    const int8_t shuffle_offset = 0, 
    const uint8_t global_delay = 0)
{
    uint8_t amount = CLIP((int8_t)time.global_shuffle + shuffle_offset, 0, 127);
    uint8_t delay = (uint32_t)((time.average_pulse_time / 16.) * (amount / 127.));
    if ((time.tick / TICKS_PER_STEP) % 2 == 1)
        return (uint32_t)((time.average_pulse_time / 8.) * (amount / 127.)) + delay;
    else
        return delay;
}
