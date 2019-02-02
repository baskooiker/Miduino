#pragma once

#include "defs.h"

void randomize_mono_dub(MonoDubSettings& settings)
{
    randomize_mono(settings.settings);
    switch (distribution(50, 10, 20))
    {
    case 0: settings.style = MonoDubStyle::MonoDubLead; break;
    case 1: settings.style = MonoDubStyle::MonoDubUnison; break;
    case 2: settings.style = MonoDubStyle::MonoDubOctave; break;
    }
}

void play_mono_dub(
    MonoDubSettings& settings, 
    const MonoSettings& lead_settings, 
    const HarmonyStruct& harmony, 
    const TimeStruct time)
{
    // Only get regular mono
    // Ignore dubbing
    bool hit = get_mono_hit(settings.settings, time);

    if (hit)
    {
        uint8_t pitch = get_mono_pitch(settings.settings, harmony, time);
        uint8_t length = 6;
        note_on(make_note(pitch, 64, length, NoteType::Tie), settings.settings.storage);
    }
}