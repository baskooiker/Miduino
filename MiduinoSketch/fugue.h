#pragma once

#include "cv.h"
#include "defs.h"
#include "gate.h"
#include "scales.h"

FuguePlayerType random_player_type()
{
    switch (randi(3))
    {
    case 1: return FuguePlayerType::FugueBackward;
    case 2: return FuguePlayerType::FugueBackAndForth;
    }
    return FuguePlayerType::FugueForward;
}

void randomize_fugue_player(FugueSettings& settings, const uint8_t id)
{
    uint8_t pitch_offsets[] = { 36, 42, 48, 54, 60, 66, 72 };
    uint8_t nr_pitch_offsets = sizeof(pitch_offsets) / sizeof(*pitch_offsets);
    randomize_order(pitch_offsets, nr_pitch_offsets);

    uint8_t lengths[] = { 1, 1, 2, 3, 4, 6, 7, 8, 10, 12, 16 };
    uint8_t nr_lengths = sizeof(lengths) / sizeof(*lengths);
    randomize_order(lengths, nr_lengths);

    NoteInterval intervals[4] = {};
    intervals[0] = NoteInterval::IntervalRoot;
    intervals[1] = NoteInterval::IntervalFifth;
    intervals[2] = random_note_interval();
    intervals[3] = random_note_interval();
    randomize_order(intervals, 4);

    for (int i = 0; i < NUMBER_FUGUE_PLAYERS; i++)
    {
        if (i != id)
        {
            remove(settings.player_settings[i].pitch_offset, pitch_offsets, nr_pitch_offsets);
            remove(settings.player_settings[i].length, lengths, nr_lengths);
        }
    }

    settings.player_settings[id % NUMBER_FUGUE_PLAYERS].pitch_offset = pitch_offsets[0];
    settings.player_settings[id % NUMBER_FUGUE_PLAYERS].length = lengths[0];
    settings.player_settings[id % NUMBER_FUGUE_PLAYERS].interval = intervals[0];
    settings.player_settings[id % NUMBER_FUGUE_PLAYERS].type = random_player_type();
}

void randomize_fugue(ApplicationData& data)
{
    // Randomize Fugue pattern
    static uint8_t pitch_opts[] = { 0, 4, 2, 3, 6 };
    data.fugue_settings.pattern.length = 0;
    uint8_t length = randi(64) < 32 ? 4 : 8;
    while (data.fugue_settings.pattern.length < length)
    {
        randomize_order(pitch_opts, 5);
        for (int i = 0; i < 5 && data.fugue_settings.pattern.length < length; i++)
        {
            data.fugue_settings.pattern.pattern[data.fugue_settings.pattern.length++] = pitch_opts[i];
        }
    }

    data.bass_settings.fugue_id = 0;
    data.bass_dub_settings.fugue_id = 1;
    data.mono_settings.fugue_id = 2;
    data.mono_dub_settings.settings.fugue_id = 3;

    for (int i = 0; i < NUMBER_FUGUE_PLAYERS; i++)
    {
        randomize_fugue_player(data.fugue_settings, i);
    }

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

void play_fugue(
    FugueSettings& fugue_settings,
    const uint8_t player_id, 
    const HarmonyStruct& harmony,
    const TimeStruct& time, 
    PitchStorage& storage)
{
    FuguePlayerSettings& player_settings = fugue_settings.player_settings[player_id % NUMBER_FUGUE_PLAYERS];
    if (time.step % player_settings.length == 0)
    { 
        uint8_t pat_length = fugue_settings.pattern.length;
        switch (player_settings.type)
        {
        case FuguePlayerType::FugueForward: 
            player_settings.counter = player_settings.counter % pat_length;
            break;
        case FuguePlayerType::FugueBackward:
            player_settings.counter = pat_length - (player_settings.counter % pat_length) - 1;
            break;
        case FuguePlayerType::FugueBackAndForth:
            player_settings.counter = player_settings.counter % (pat_length * 2);
            if (player_settings.counter < pat_length)
            {
                player_settings.counter = player_settings.counter % pat_length;
            }
            else
            {
                player_settings.counter = pat_length - (player_settings.counter % pat_length) - 1;
            }
            break;
        }

        uint8_t note_step = cv(fugue_settings.pattern, player_settings.counter);
        player_settings.counter++;
        uint8_t pitch = apply_scale_offset(note_step, harmony.scale, player_settings.pitch_offset);
        uint8_t length = MAX(player_settings.length - 1, 1) * TICKS_PER_STEP;
        note_on(make_note(pitch, 64, 6, NoteType::Tie), storage);
    }
}