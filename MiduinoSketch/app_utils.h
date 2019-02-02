#pragma once

#include "note_handling.h"

#include "lead.h"
#include "mfb_503.h"
#include "mfb_522.h"
#include "mono.h"
#include "poly.h"
#include "bass.h"
#include "bass_dub.h"

void play_all(ApplicationData& data)
{
    play_503(data);
    play_522(data);
    play_bass(data);
    play_bass_dub(data.bass_settings, data.bass_dub_settings, data.harmony, data.step, data.ticks);
    play_poly(data);
    play_lead(data);
    play_mono(data.mono_settings, data.harmony, data.step, data.ticks);
    play_mono(data.mono_2_settings, data.harmony, data.step, data.ticks);
}

void stop_notes_all_instruments(ApplicationData& data)
{
    stop_notes(data.mfb_503_settings.storage);
    stop_notes(data.mfb_522_settings.storage);
    stop_notes(data.poly_settings.storage);
    stop_notes(data.bass_settings.storage);
    stop_notes(data.lead_settings.storage);
    stop_notes(data.mono_settings.storage);
    stop_notes(data.mono_2_settings.storage);
}

void all_notes_off(PitchStorage& storage)
{
    NoteStruct p = { 0, 0, 0 };
    do {
        p = pop_from_storage(storage);
        if (p.pitch > 0)
        {
            note_off(p.pitch, storage);
        }
    } while (p.pitch != 0);
}

void randomize_all(ApplicationData& data)
{
    randomize_harmony(data);

    randomize_503_seq(data);
    randomize_522_seq(data);
    randomize_poly(data);
    randomize_bass(data);
    randomize_lead(data);
    randomize_mono(data.mono_settings);
    randomize_mono(data.mono_2_settings);

    randomize_503_sound(data);
}
