#include "application/ui.h"

#include "utils/utils.h"

namespace Vleerhond {
void ButtonState::press() { this->last_pressed = Utils::millis(); }

void ButtonState::release() { this->last_released = Utils::millis(); }

bool ButtonState::is_pressed() const { return last_released < last_pressed; }

uint32_t ButtonState::time_since_release() const {
    return Utils::millis() - last_released;
}

uint32_t ButtonState::time_since_press() const {
    return Utils::millis() - last_pressed;
}

bool ButtonState::was_pressed_long() const {
    unsigned long t = Utils::millis();
    if (t > last_pressed) {
        return (t - last_pressed) > 500;
    }
    return true;
}

bool ButtonState::long_since_release() const {
    return Utils::millis() - last_released > 500;
}

////////////////////////////////////////////////////////////////////////

bool UiState::is_pressed(const uint8_t id) {
    return get_step_state(id).is_pressed();
}

void UiState::release_step(const uint8_t id) { get_step_state(id).release(); }

void UiState::press_step(const uint8_t id) { get_step_state(id).press(); }

ButtonState& UiState::get_step_state(const uint8_t id) {
    if (step_states.find(id) == step_states.end()) {
        step_states.emplace(id, ButtonState());
    }
    return step_states[id];
}

uint8_t UiState::getValue(const uint8_t id) {
    return 0;
    // return get_step_state(id).value();
}
}  // namespace Vleerhond
