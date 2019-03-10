#pragma once

#include "defs.h"

void randomize_mono_dub(MonoDubSettings& settings)
{
    randomize_mono(settings.settings);
}

void play_mono_dub(
    ApplicationData& data,
    MonoDubSettings& settings, 
    const MonoSettings& lead_settings, 
    HarmonyStruct& harmony, 
    TimeStruct time)
{
    bool hit = false;
    switch (settings.style)
    {
    case MonoDubStyle::MonoDubLead: 
        return play_mono(data, settings.settings, harmony, time);
        break;
    case MonoDubStyle::MonoDubUnison:
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
        }

        pitch = clip_pitch(pitch, apply_cv(settings.variable_pitch_offset, 48, 60));

        note_on(
            make_note(pitch, 64, 6, NoteType::Tie), 
            settings.settings.storage, 
            get_shuffle_delay(time)
        );
    }
}