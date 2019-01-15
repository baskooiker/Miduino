#pragma once

void set_eights(BinaryPattern& pattern, const uint8_t _steps)
{
    pattern = 0x00;

    uint8_t steps = MIN(_steps, 8);
    uint8_t step_options[8] = { 0 };
    for (int i = 0; i < 8; i++)
    {
        step_options[i] = i * 2;
    }
    randomize_order(step_options, 8);

    for (int i = 0; i < steps; i++)
    {
        set_gate(pattern, step_options[i]);
    }
}

void get_hit_indices(const BinaryPattern pattern, uint8_t* hits, uint8_t& size)
{
    size = 0;
    for (int i = 0; i < 16; i++) 
    {
        if (gate(pattern, i))
            hits[size++] = i;
    }
}

