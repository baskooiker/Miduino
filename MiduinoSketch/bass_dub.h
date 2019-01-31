#pragma once

#include "defs.h"
#include "bass.h"
#include "midi_io.h"

void randomize_bass_dub(BassDubSettings& settings)
{
    switch (distribution(10, 30))
    {
    case 0: settings.style = BassDubStyle::Unison; break;
    case 1: settings.style = BassDubStyle::Octave; break;
    }
}

void play_bass_dub(
    BassSettings& settings, 
    BassDubSettings& dub_settings, 
    const HarmonyStruct harmony,
    const uint32_t step, 
    const uint8_t tick)
{
    bool hit = false;
    switch (dub_settings.style)
    {
    case BassDubStyle::Unison:
    case BassDubStyle::Octave:
        hit = get_bass_hit(settings, step, tick);
        break;
    }

    if (hit)
    {
        uint8_t pitch = get_bass_pitch(settings, harmony, step, tick);

        switch (dub_settings.style)
        {
        case BassDubStyle::Unison:
            break;
        case BassDubStyle::Octave:
            pitch += 12;
            break;
        }

        note_on(make_note(pitch, 64, 6, NoteType::Tie), dub_settings.storage);
    }
}
