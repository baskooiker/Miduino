#pragma once

uint8_t randi(const uint8_t max)
{
    if (max == 0)
    {
        return 0;
    }
    return rand() % max;
}

uint8_t randi(const uint8_t min, const uint8_t max)
{
    uint8_t _min = MIN(min, max);
    uint8_t _max = MAX(min, max);
    return _min + randi(_max - _min);
}

float randf(const float max = 1.f)
{
    // TODO
    return ((float)rand() / (float)(RAND_MAX)) * max;
}

float randf(const float min, const float max)
{
    // TODO
    float _min = MIN(min, max);
    float _max = MAX(min, max);
    return _min + randf(_max - _min);
}

