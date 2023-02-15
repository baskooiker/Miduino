#pragma once

#include "core/defs.h"

namespace Vleerhond {
class ButtonState {
   private:
    unsigned long last_pressed;
    unsigned long last_released;

   public:
    void press();
    void release();
    bool is_pressed() const;
    uint32_t time_since_release() const;
    uint32_t time_since_press() const;
    bool was_pressed_long() const;
    bool long_since_release() const;
};

class UiState {
   private:
    std::map<uint8_t, ButtonState> step_states;

   public:
    bool is_pressed(const uint8_t id);
    void release_step(const uint8_t id);
    void press_step(const uint8_t id);
    ButtonState& get_step_state(const uint8_t id);
    uint8_t getValue(const uint8_t id);
};
}  // namespace Vleerhond
