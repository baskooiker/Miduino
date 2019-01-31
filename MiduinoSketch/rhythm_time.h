#pragma once

uint8_t ticks_left_in_bar(const uint32_t step, const uint8_t tick)
{
    static const uint32_t ticks_in_bar = 16 * TICKS_PER_STEP;
    uint32_t tick_in_local_bar = (step * TICKS_PER_STEP + tick) % ticks_in_bar;
    return ticks_in_bar - tick_in_local_bar - 1;
}