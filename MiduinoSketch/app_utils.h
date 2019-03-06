#pragma once

#include "note_handling.h"

#include "lead.h"
#include "mfb_503.h"
#include "tanzbar.h"
#include "mfb_522.h"
#include "mono.h"
#include "mono_dub.h"
#include "poly.h"
#include "bass.h"
#include "bass_dub.h"

void play_all(ApplicationData& data)
{
    //play_503(data.mfb_503_settings, data.harmony, data.time);
    play_tanzbar(data.tanzbar_settings, data.time);
    play_bass(data, data.time);
    play_bass_dub(
        data,
        data.bass_settings, 
        data.bass_dub_settings, 
        data.harmony, 
        data.time);
    //play_poly(data, data.time);
    //play_lead(data, data.time);
    play_mono(data, data.mono_settings, data.harmony, data.time);
    play_mono_dub(data, data.mono_dub_settings, data.mono_settings, data.harmony, data.time);
}

void stop_notes_all_instruments(ApplicationData& data)
{
    stop_notes(data.tanzbar_settings.storage);

    stop_notes(data.bass_settings.storage);
    stop_notes(data.bass_dub_settings.storage);

    stop_notes(data.mono_settings.storage);
    stop_notes(data.mono_dub_settings.settings.storage);
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
    randomize_fugue(data.fugue_settings);
    
    randomize_tanzbar(data.tanzbar_settings);
    randomize_bass(data.bass_settings);
    randomize_bass_dub(data.bass_dub_settings);

    randomize_mono(data.mono_settings);
    randomize_mono_dub(data.mono_dub_settings);

    randomize_tanzbar_sound(data.tanzbar_settings);
}

void set_fugue(ApplicationData& data)
{
    randomize_fugue(data.fugue_settings);
    data.tanzbar_settings.kill_low = true;
    data.tanzbar_settings.kill_mid = true;
    data.tanzbar_settings.kill_perc = true;
    data.tanzbar_settings.kill_hats = true;

    // Set bass
    data.bass_settings.style = BassStyle::BassFugue;
    // Set bass dub
    data.bass_dub_settings.style = BassDubStyle::DubFugue;
    // Set mono
    data.mono_settings.style = MonoStyle::MonoFugue;
    // Set mono dub
    data.mono_dub_settings.style = MonoDubStyle::MonoDubLead;
    data.mono_dub_settings.settings.style = MonoStyle::MonoFugue;
}

void process_events(PitchStorage& storage)
{
    uint32_t time = millis();
    for (int i = storage.nr_of_events - 1; i >= 0; i--)
    {
        NoteEvent event_i = storage.events[i];
        if (event_i.time <= time)
        {
            note_on(event_i.note, storage);
            storage.events[i] = storage.events[storage.nr_of_events - 1];
            storage.nr_of_events--;
        }
    }
}

void process_events(ApplicationData& data)
{
    process_events(data.tanzbar_settings.storage);
    process_events(data.bass_settings.storage);
    process_events(data.bass_dub_settings.storage);
    process_events(data.mono_settings.storage);
    process_events(data.mono_dub_settings.settings.storage);
}
