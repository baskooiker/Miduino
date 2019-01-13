#pragma once

#include "defs.h"
#include "midi_io.h"

void add_to_storage(PitchStorage& s, uint8_t pitch, uint8_t length = HOLD_NOTE, const uint8_t velocity = 64)
{
    for (uint8_t i = 0; i < s.size; i++)
    {
        if (s.data[i].pitch == pitch)
        {
            s.data[i].length = length;
            return;
        }
    }
    s.data[s.size++] = { pitch, velocity, length };
}

NoteStruct pop_from_storage(PitchStorage& s)
{
	if (s.size > 0) {
        NoteStruct v = s.data[s.size];
        s.data[s.size--] = { 0, 0, 0 };
		return v;
	}
    return {0, 0, 0};
}

NoteStruct pop_from_storage(PitchStorage& s, uint8_t pitch)
{
	for (uint8_t i = 0; i < s.size; i++)
	{
		if (s.data[i].pitch == pitch) {
            NoteStruct v = s.data[i];
            s.data[i] = s.data[s.size - 1];
            s.size--;
			return v;
		}
	}
    return {0, 0, 0};
}

void stop_notes(PitchStorage& storage, uint8_t channel)
{
    if (storage.size == 0) return;
    for (uint8_t i = 0; i < storage.size; i++)
    {
        if (storage.data[i].length != HOLD_NOTE
            && storage.data[i].length != TIE_NOTE
            && storage.data[i].length > 0)
        {
            storage.data[i].length -= 1;
        }
    }

    for (uint8_t i = 0; i < storage.size; i++)
    {
        if (storage.data[i].length == 0)
        {
            note_off(storage.data[i].pitch, channel, storage);
        }
    }
}

void untie_notes(PitchStorage& storage)
{
    for (uint8_t i = 0; i < storage.size; i++)
    {
        if (storage.data[i].length == TIE_NOTE)
        {
            storage.data[i].length = 0;
        }
    }
}