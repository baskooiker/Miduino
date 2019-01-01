#pragma once

void note_on(uint8_t note, uint8_t velocity, uint8_t channel, PitchStorage& storage, uint8_t length=HOLD_NOTE);
void note_off(uint8_t note, uint8_t channel, PitchStorage& storage);
void send_cc(uint8_t cc, uint8_t value, uint8_t channel);
void all_notes_off(PitchStorage& storage, uint8_t channel);
