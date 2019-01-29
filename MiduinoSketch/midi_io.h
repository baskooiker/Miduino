#pragma once

#include "defs.h"

void note_on(const NoteStruct note, PitchStorage& storage);
void note_on(const NoteStruct* note, uint8_t length, PitchStorage& storage);
void note_off(uint8_t note, PitchStorage& storage);
void send_cc(uint8_t cc, uint8_t value, uint8_t channel);
void all_notes_off(PitchStorage& storage);
