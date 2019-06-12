#pragma once

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
    case BTN_RIGHT_BTM_01: return 0;
    case BTN_RIGHT_BTM_02: return 1;
    case BTN_RIGHT_BTM_03: return 2;
    case BTN_RIGHT_BTM_04: return 3;
    case BTN_RIGHT_BTM_05: return 4;
    case BTN_RIGHT_BTM_06: return 5;
    case BTN_RIGHT_BTM_07: return 6;
    case BTN_RIGHT_BTM_08: return 7;
    case BTN_RIGHT_TOP_01: return 8;
    case BTN_RIGHT_TOP_02: return 9;
    case BTN_RIGHT_TOP_03: return 10;
    case BTN_RIGHT_TOP_04: return 11;
    case BTN_RIGHT_TOP_05: return 12;
    case BTN_RIGHT_TOP_06: return 13;
    case BTN_RIGHT_TOP_07: return 14;
    case BTN_RIGHT_TOP_08: return 15;
    }
    return 0xFF;
}

uint8_t get_step_index(uint8_t id)
{
    switch (id)
    {
    case BTN_LEFT_TOP_01: return 0;
    case BTN_LEFT_TOP_02: return 1;
    case BTN_LEFT_TOP_03: return 2;
    case BTN_LEFT_TOP_04: return 3;
    case BTN_LEFT_TOP_05: return 4;
    case BTN_LEFT_TOP_06: return 5;
    case BTN_LEFT_TOP_07: return 6;
    case BTN_LEFT_TOP_08: return 7;
    case BTN_LEFT_BTM_01: return 8;
    case BTN_LEFT_BTM_02: return 9;
    case BTN_LEFT_BTM_03: return 10;
    case BTN_LEFT_BTM_04: return 11;
    case BTN_LEFT_BTM_05: return 12;
    case BTN_LEFT_BTM_06: return 13;
    case BTN_LEFT_BTM_07: return 14;
    case BTN_LEFT_BTM_08: return 15;
    }
    return 0xFF;
}

void release_step(std::map<uint8_t, ButtonState>& states, uint8_t id)
{
    if (states.find(id) == states.end())
    {
        states.emplace(id, ButtonState());
    }
    {
        states[id].last_released = millis();
    }
}

void press_step(std::map<uint8_t, ButtonState>& states, uint8_t id)
{
    if (states.find(id) == states.end())
    {
        states.emplace(id, ButtonState());
    }
    states[id].last_pressed = millis();
}

uint32_t time_since_press(const ButtonState& state)
{
    return millis() - state.last_pressed;
}

uint32_t time_since_release(const ButtonState& state)
{
    return millis() - state.last_released;
}

const ButtonState& get_step_state(std::map<uint8_t, ButtonState>& states, uint8_t id)
{
    if (states.find(id) == states.end())
    {
        states.emplace(id, ButtonState());
    }
    return states[id];
}

bool is_pressed(const ButtonState& state)
{
    return state.last_released < state.last_pressed;
}
