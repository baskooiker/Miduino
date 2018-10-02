#pragma once

#include "defs.h"

void add_to_storage(PitchStorage& s, uint8_t value)
{
	for (uint8_t i = 0; i < STORAGE_SIZE; i++)
	{
		if (s[i] == 0) {

			s[i] = value;
			break;
		}
	}
}

uint8_t pop_from_storage(PitchStorage& s)
{
	for (uint8_t i = 0; i < STORAGE_SIZE; i++)
	{
		uint8_t v = s[i];
		if (v > 0) {
			s[i] = 0;
			return v;
		}
	}
	return 0;
}

uint8_t pop_from_storage(uint8_t* s, uint8_t value)
{
	for (uint8_t i = 0; i < STORAGE_SIZE; i++)
	{
		uint8_t v = s[i];
		if (s[i] == value) {
			s[i] = 0;
			return v;
		}
	}
	return 0;
}

