#pragma once

uint8_t to_chord_order(const uint8_t order)
{
    if (order < 2)
        return 0;
    if (order < 4)
        return 2;
    if (order < 6)
        return 4;
    return 0;
}
