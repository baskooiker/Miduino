#pragma once

#include "storage.h"

#include "lead.h"
#include "mfb_503.h"
#include "mfb_522.h"
#include "mono.h"
#include "p50.h"
#include "rocket.h"

void play_all(ApplicationData& data)
{
    play_503(data);
    play_522(data);
    play_rocket(data);
    play_P50(data);
    play_lead(data);
    play_mono(data);
}

void stop_notes_all_instruments(ApplicationData& data)
{
    stop_notes(data.settings_503.storage, MIDI_CHANNEL_503);
    stop_notes(data.settings_522.storage, MIDI_CHANNEL_522);
    stop_notes(data.settings_p50.storage, MIDI_CHANNEL_P50);
    stop_notes(data.settings_rocket.storage, MIDI_CHANNEL_ROCKET);
    stop_notes(data.settings_lead.storage, MIDI_CHANNEL_LEAD);
    stop_notes(data.settings_mono.storage, MIDI_CHANNEL_MONO);
}

void all_notes_off(PitchStorage& storage, uint8_t channel)
{
    NoteStruct p = { 0, 0, 0 };
    do {
        p = pop_from_storage(storage);
        if (p.pitch > 0)
        {
            note_off(p.pitch, channel, storage);
        }
    } while (p.pitch != 0);
}

void randomize_all(ApplicationData& data)
{
    randomize_harmony(data);

    randomize_503_seq(data);
    randomize_522_seq(data);
    randomize_P50_seq(data);
    randomize_rocket_seq(data);
    randomize_lead(data);
    randomize_mono(data);

    randomize_503_sound(data);
}
