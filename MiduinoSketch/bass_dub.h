#pragma once

#include "bass_dub_settings.h"

#include "defs.h"
#include "bass.h"
#include "midi_io.h"

void randomize_bass_dub(BassDubSettings& settings)
{
    switch (distribution(0, 30, 10))
    {
    case 0: 
        settings.style = BassDubStyle::DubUnison; 
        break;
    case 1: 
        settings.style = BassDubStyle::DubOctave; 
        break;
    case 2: 
        settings.style = BassDubStyle::DubOctProbability; 
        break;
    }
    switch (distribution(60, 60))
    {
    case 0: 
        settings.note_interval = NoteInterval::IntervalThird; 
        break;
    case 1: 
        settings.note_interval = NoteInterval::IntervalFifth; 
        break;
    }
    settings.hit_probs.randomize(randf(.25f, .75f));
}

void play_bass_dub(
    ApplicationData& data,
    BassSettings& settings, 
    BassDubSettings& dub_settings, 
    HarmonyStruct harmony,
    TimeStruct& time)
{
    if (dub_settings.style == BassDubStyle::DubFugue)
    {
        return play_fugue(
            data.fugue_settings,
            dub_settings.fugue_id, 
            harmony, 
            time, 
            dub_settings.storage);
    }

    bool hit = get_bass_hit(settings,dub_settings.density, time);
    if (hit)
    {
        NoteInterval note_interval = NoteInterval::IntervalRoot;
        switch (dub_settings.style)
        {
        case BassDubStyle::DubOctave: note_interval = dub_settings.note_interval; break;
        default: break;
        }

        uint8_t pitch = get_bass_pitch(
            settings, 
            harmony, 
            time, 
            dub_settings.v_pitch, 
            note_interval
        );

        switch (dub_settings.style)
        {
        default:
        case BassDubStyle::DubUnison:
        case BassDubStyle::DubOctave:
            pitch = clip_pitch(pitch, rerange(dub_settings.v_pitch, 36, 48));
            break;
        case BassDubStyle::DubOctProbability:
            //pitch = clip_pitch(pitch, 36, rerange(dub_settings.v_pitch, 36, 48));
            break;
        }

        dub_settings.storage.note_on(
            make_note(pitch, 64, 6, NoteType::Tie), 
            get_shuffle_delay(time)
        );
    }
}
