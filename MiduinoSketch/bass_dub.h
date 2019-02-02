#pragma once

#include "defs.h"
#include "bass.h"
#include "midi_io.h"
#include "rhythms.h"

void randomize_bass_dub(BassDubSettings& settings)
{
    switch (distribution(0, 30, 10, 10))
    {
    case 0: settings.style = BassDubStyle::DubUnison; break;
    case 1: settings.style = BassDubStyle::DubOctave; break;
    case 2: settings.style = BassDubStyle::DubOctProbability; break;
    case 3: settings.style = BassDubStyle::DubHitProbability; break;
    }
    switch (distribution(60, 10, 20))
    {
    case 0: settings.note_interval = NoteInterval::IntervalRoot; break;
    case 1: settings.note_interval = NoteInterval::IntervalThird; break;
    case 2: settings.note_interval = NoteInterval::IntervalFifth; break;
    }
    randomize(settings.octave_probs, randf(.25f, .75f));
    randomize(settings.hit_probs, randf(.25f, .75f));
}

void play_bass_dub(
    BassSettings& settings, 
    BassDubSettings& dub_settings, 
    const HarmonyStruct harmony,
    const TimeStruct& time)
{
    bool hit = get_bass_hit(settings, time);
    switch (dub_settings.style)
    {
    case BassDubStyle::DubUnison:
    case BassDubStyle::DubOctave:
    case BassDubStyle::DubOctProbability:
        break;
    case BassDubStyle::DubHitProbability:
        if (hit)
        {
            hit = gate(dub_settings.hit_probs, time);
        }
        break;
    }

    if (hit)
    {
        uint8_t pitch = get_bass_pitch(settings, harmony, time);

        switch (dub_settings.style)
        {
        case BassDubStyle::DubUnison:
            break;
        case BassDubStyle::DubHitProbability:
        case BassDubStyle::DubOctave:
            pitch += 12;
            break;
        case BassDubStyle::DubOctProbability:
            if (gate(dub_settings.octave_probs, time))
            {
                pitch += 12;
            }
            break;
        }

        note_on(make_note(pitch, 64, 6, NoteType::Tie), dub_settings.storage);
    }
}
