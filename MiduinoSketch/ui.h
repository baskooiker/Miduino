#pragma once

#include "rhythms.h"

void set_button_state(UiState& state, uint8_t index, bool value)
{
    if (value)
    {
        state.bsp_button_state |= ((uint16_t)(0x1) << index);
    }
    else
    {
        state.bsp_button_state &= ~((uint16_t)(0x1) << index);
    }
}

bool get_button_state(const UiState& state, uint8_t index)
{
    return (((state.bsp_button_state) & (uint16_t)(0x1 << (uint16_t)(index % 16)))) > 0;
}

void set_pad_state(UiState& state, uint8_t index, bool value)
{
    if (value)
    {
        state.bsp_pad_state |= ((uint16_t)(0x1) << index);
    }
    else
    {
        state.bsp_pad_state &= ~((uint16_t)(0x1) << index);
    }
}

bool get_pad_state(const UiState& state, uint8_t index)
{
    return (((state.bsp_pad_state) & (uint16_t)(0x1 << (uint16_t)(index % 16)))) > 0;
}
