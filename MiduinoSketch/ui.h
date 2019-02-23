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

//bool get_button_state(const UiState& state, uint8_t index)
//{
//    return (((state.bsp_button_state) & (uint16_t)(0x1 << (uint16_t)(index % 16)))) > 0;
//}

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

bool was_pressed_long(ButtonState& state)
{
    unsigned long t = millis();
    if (t > state.last_pressed)
    {
        return (t - state.last_pressed) > 500;
    }
    return true;
}

uint8_t get_pad_index(uint8_t id)
{
    switch (id)
    {
    case BSP_PAD_01: return 0;
    case BSP_PAD_02: return 1;
    case BSP_PAD_03: return 2;
    case BSP_PAD_04: return 3;
    case BSP_PAD_05: return 4;
    case BSP_PAD_06: return 5;
    case BSP_PAD_07: return 6;
    case BSP_PAD_08: return 7;
    case BSP_PAD_09: return 8;
    case BSP_PAD_10: return 9;
    case BSP_PAD_11: return 10;
    case BSP_PAD_12: return 11;
    case BSP_PAD_13: return 12;
    case BSP_PAD_14: return 13;
    case BSP_PAD_15: return 14;
    case BSP_PAD_16: return 15;
    }
    return 0xFF;
}

uint8_t get_step_index(uint8_t id)
{
    switch (id)
    {
    case BSP_STEP_01: return 0;
    case BSP_STEP_02: return 1;
    case BSP_STEP_03: return 2;
    case BSP_STEP_04: return 3;
    case BSP_STEP_05: return 4;
    case BSP_STEP_06: return 5;
    case BSP_STEP_07: return 6;
    case BSP_STEP_08: return 7;
    case BSP_STEP_09: return 8;
    case BSP_STEP_10: return 9;
    case BSP_STEP_11: return 10;
    case BSP_STEP_12: return 11;
    case BSP_STEP_13: return 12;
    case BSP_STEP_14: return 13;
    case BSP_STEP_15: return 14;
    case BSP_STEP_16: return 15;
    }
    return 0xFF;
}

void release_pad(ButtonState* states, uint8_t id)
{
    uint8_t idx = get_pad_index(id);
    if (idx < NR_OF_PADS)
    {
        states[idx].last_released = millis();
    }
}

void press_pad(ButtonState* states, uint8_t id)
{
    uint8_t idx = get_pad_index(id);
    if (idx < NR_OF_PADS)
    {
        states[idx].last_pressed = millis();
    }
}

void release_step(ButtonState* states, uint8_t id)
{
    uint8_t idx = get_step_index(id);
    if (idx < NR_OF_STEPS)
    {
        states[idx].last_released = millis();
    }
}

void press_step(ButtonState* states, uint8_t id)
{
    uint8_t idx = get_step_index(id);
    if (idx < NR_OF_STEPS)
    {
        states[idx].last_pressed = millis();
    }
}

uint32_t time_since_press(const ButtonState& state)
{
    return millis() - state.last_pressed;
}

uint32_t time_since_release(const ButtonState& state)
{
    return millis() - state.last_released;
}

const ButtonState& get_step_state(const ButtonState* state, uint8_t id)
{
    return state[get_step_index(id)];
}

const ButtonState& get_pad_state(const ButtonState* state, uint8_t id)
{
    return state[get_pad_index(id)];
}

bool is_pressed(const ButtonState& state)
{
    return state.last_released < state.last_pressed;
}
