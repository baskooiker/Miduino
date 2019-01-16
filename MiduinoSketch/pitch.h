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

uint8_t clip_pitch(const uint8_t pitch, const uint8_t min, const uint8_t _max)
{
    uint8_t max = MAX(min + 12, _max);
    if (pitch < min)
        return clip_pitch(pitch + 12, min, max);
    if (pitch > max)
        return clip_pitch(pitch - 12, min, max);
    return pitch;
}
