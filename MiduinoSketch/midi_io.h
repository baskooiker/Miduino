#pragma once

#include "defs.h"

void note_on(const NoteStruct note, PitchStorage& storage);
void note_on(const NoteStruct* note, const uint8_t length, PitchStorage& storage);
void note_off(const uint8_t note, PitchStorage& storage);
void send_cc(const uint8_t cc, const uint8_t value, const uint8_t channel);
void all_notes_off(PitchStorage& storage);
