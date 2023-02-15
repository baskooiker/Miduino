#pragma once

#include "application_data.h"
#include "core/defs.h"
#include "harmony/chords.h"
#include "rand.h"
#include "ui.h"

namespace Vleerhond {
void handleClock(ApplicationData& data);
void handle_step_release(
    ApplicationData& data, uint8_t released_button, uint8_t button_1,
    uint8_t button_2, void callback_one(ApplicationData&),
    void callback_two(ApplicationData&), void callback_both(ApplicationData&));
void handleControlChangePlaying(
    ApplicationData& data, uint8_t channel, uint8_t number, uint8_t value);
void handleControlChangeStopped(
    ApplicationData& data, uint8_t channel, uint8_t number, uint8_t value);
void handleControlChange(
    ApplicationData& data, uint8_t channel, uint8_t number, uint8_t value);
void handleStop(ApplicationData& data);
}  // namespace Vleerhond