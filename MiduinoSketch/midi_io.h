#pragma once

void send_note_on(const uint8_t pitch, const uint8_t velocity, const uint8_t channel);
void send_note_off(const uint8_t pitch, const uint8_t channel);
void send_cc(const uint8_t cc, const uint8_t value, const uint8_t channel);
