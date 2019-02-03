#pragma once

#include "cv.h"
#include "defs.h"
#include "gate.h"
#include "scales.h"

FuguePlayerType random_player_type()
{
    // TODO: Remove this early out!
    return FuguePlayerType::FugueForward;
    switch (randi(3))
    {
    case 1: return FuguePlayerType::FugueBackward;
    case 2: return FuguePlayerType::FugueBackAndForth;
    }
    return FuguePlayerType::FugueForward;
}

void randomize_fugue(ApplicationData& data)
{
    // Randomize Fugue pattern
    uint8_t pitches[] = { 0, 2, 3, 4, 6 };
    randomize_order(pitches, 5);
    for (int i = 0; i < 4; i ++)
    {
        data.fugue_settings.pattern.pattern[i] = pitches[i];
    }
    data.fugue_settings.pattern.length = 4;

    uint8_t pitch_offsets[] = { 30, 36, 42, 48, 54, 60, 66 };
    uint8_t lengths[] = { 2, 4, 6, 7, 8, 10, 12, 14, 16 };
    randomize_order(pitch_offsets, 7);
    randomize_order(lengths, 9);

    // Set bass
    data.bass_settings.style = BassStyle::BassFugue;
    data.bass_settings.fugue_player_settings.pitch_offset = pitch_offsets[0];
    data.bass_settings.fugue_player_settings.length = lengths[0];
    data.bass_settings.fugue_player_settings.type = random_player_type();

    // Set bass dub
    data.bass_dub_settings.style = BassDubStyle::DubFugue;
    data.bass_dub_settings.fugue_player_settings.pitch_offset = pitch_offsets[1];
    data.bass_dub_settings.fugue_player_settings.length = lengths[1];
    data.bass_dub_settings.fugue_player_settings.type = random_player_type();

    // Set mono
    data.mono_settings.style = MonoStyle::MonoFugue;
    data.mono_settings.fugue_player_settings.pitch_offset = pitch_offsets[2];
    data.mono_settings.fugue_player_settings.length = lengths[2];
    data.mono_settings.fugue_player_settings.type = random_player_type();

    // Set mono dub
    data.mono_dub_settings.style = MonoDubStyle::MonoDubLead;
    data.mono_dub_settings.settings.style = MonoStyle::MonoFugue;
    data.mono_dub_settings.settings.fugue_player_settings.pitch_offset = pitch_offsets[3];
    data.mono_dub_settings.settings.fugue_player_settings.length = lengths[3];
    data.mono_dub_settings.settings.fugue_player_settings.type = random_player_type();
}

void play_fugue(
    FugueSettings& fugue_settings,
    FuguePlayerSettings& player_settings, 
    const HarmonyStruct& harmony,
    const TimeStruct& time, 
    PitchStorage& storage)
{
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