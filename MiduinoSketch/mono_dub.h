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
    ApplicationData& data,
    MonoDubSettings& settings, 
    const MonoSettings& lead_settings, 
    const HarmonyStruct& harmony, 
    const TimeStruct time)
{
    bool hit = false;
    switch (settings.style)
    {
    case MonoDubStyle::MonoDubLead: 
        if (settings.settings.style == MonoStyle::MonoFugue && time.tick == 0)
        {
            return play_fugue(
                data.fugue_settings,
                settings.settings.fugue_player_settings,
                harmony,
                time,
                settings.settings.storage);
        }
        hit = get_mono_hit(settings.settings, time);  
        break;
    case MonoDubStyle::MonoDubUnison: 
    case MonoDubStyle::MonoDubOctave:
        hit = get_mono_hit(lead_settings, time); 
        break;
    }

    if (hit)
    {
        uint8_t pitch = 0;

        switch (settings.style)
        {
        case MonoDubStyle::MonoDubLead:
            pitch = get_next_mono_pitch(settings.settings, harmony, time);
            break;
        case MonoDubStyle::MonoDubUnison:
            pitch = get_mono_pitch(lead_settings, harmony, time);
            break;
        case MonoDubStyle::MonoDubOctave:
            pitch = get_mono_pitch(lead_settings, harmony, time);
            pitch += 12;
            break;
        }

        uint8_t length = 6;
        note_on(make_note(pitch, 64, length, NoteType::Tie), settings.settings.storage);
    }
}