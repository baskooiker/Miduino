#pragma once

#include "cv.h"
#include "defs.h"
#include "harmony_struct.h"
#include "gate.h"

uint8_t time_intervals[] = { 1, 2, 3, 4, 5, 6, 7, 8, 10, 12, 14, 16 };
uint8_t nr_time_intervals = sizeof(time_intervals) / sizeof(*time_intervals);

FuguePlayerType random_player_type()
{
    switch (randui8(3))
    {
    default:
    case 0: return FuguePlayerType::FugueForward;
    case 1: return FuguePlayerType::FugueBackward;
    case 2: return FuguePlayerType::FugueBackAndForth;
    }
}

void randomize_fugue_player(FugueSettings& settings, const uint8_t id)
{
    uint8_t pitch_offsets[] = { 36, 40, 44, 48, 52 };
    uint8_t nr_pitch_offsets = sizeof(pitch_offsets) / sizeof(*pitch_offsets);
    randomize_order(pitch_offsets, nr_pitch_offsets);

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
        }
    }

    FuguePlayerSettings& player_settings = settings.player_settings[id % NUMBER_FUGUE_PLAYERS];

    player_settings.pitch_offset = pitch_offsets[0];
    player_settings.length = randui8(2, nr_time_intervals - 1);
    player_settings.type = random_player_type();
    player_settings.note_interval = intervals[0];

    switch (distribution(32, 4, 4, 4, 4))
    {
    case 0: player_settings.rhythm = BXXXX; break;
    case 1: player_settings.rhythm = BXXX0; break;
    case 2: player_settings.rhythm = BXX0X; break;
    case 3: player_settings.rhythm = BX0XX; break;
    case 4: player_settings.rhythm = B0XXX; break;
    }

    switch (distribution(32, 4, 4, 4))
    {
    case 0: player_settings.note_repeat = 1; break;
    case 1: player_settings.note_repeat = 2; break;
    case 2: player_settings.note_repeat = 3; break;
    case 3: player_settings.note_repeat = 4; break;
    }
}

void randomize_fugue(FugueSettings& settings)
{
    // Randomize Fugue pattern
    static uint8_t pitch_opts[] = { 0, 4, 2, 3, 6 };
    settings.pattern.length = 0;
    uint8_t length = randui8(64) < 32 ? 4 : 8;
    while (settings.pattern.length < length)
    {
        randomize_order(pitch_opts, 5);
        for (int i = 0; i < 5 && settings.pattern.length < length; i++)
        {
            settings.pattern.pattern.set(settings.pattern.length++, pitch_opts[i]);
        }
    }

    for (int i = 0; i < NUMBER_FUGUE_PLAYERS; i++)
    {
        randomize_fugue_player(settings, i);
    }
}

void play_fugue(
    FugueSettings& fugue_settings,
    const uint8_t player_id, 
    HarmonyStruct& harmony,
    const TimeStruct& time, 
    PitchStorage& storage)
{
    if (interval_hit(TimeDivision::Sixteenth, time))
    {
        FuguePlayerSettings& player_settings = fugue_settings.player_settings[player_id % NUMBER_FUGUE_PLAYERS];
        bool hit = false;

        uint8_t player_length_idx = apply_cv(player_settings.density, 5, player_settings.length - 2);
        uint32_t player_length = time_intervals[MAX(MIN(player_length_idx, nr_time_intervals), 0)];

        if (player_settings.counter % player_length == 0)
        {
            hit = gate(player_settings.rhythm, player_settings.counter / player_length, 4);
        }
        uint8_t pat_length = fugue_settings.pattern.length * player_settings.note_repeat;
        uint32_t hit_count = (player_settings.counter / player_length) / player_settings.note_repeat;

        if (hit)
        {
            uint32_t c = hit_count;
            switch (player_settings.type)
            {
            case FuguePlayerType::FugueForward:
                c = hit_count % pat_length;
                break;
            case FuguePlayerType::FugueBackward:
                c = pat_length - (c % pat_length) - 1;
                break;
            case FuguePlayerType::FugueBackAndForth:
                c = c % (pat_length * 2);
                if (c < pat_length)
                {
                    c = c % pat_length;
                }
                else
                {
                    c = pat_length - (c % pat_length) - 1;
                }
                break;
            }

            uint8_t note_step = fugue_settings.pattern.cv(c);

            uint8_t pitch_offset = apply_cv(player_settings.manual_pitch_offset, 36, player_settings.pitch_offset);
            uint8_t pitch = harmony.scale.apply_scale_offset(
                note_step, 
                pitch_offset,
                player_settings.note_interval
            );
            note_on(
                make_note(pitch, 64, player_length * TICKS_PER_STEP, NoteType::Tie),
                storage, 
                get_shuffle_delay(time)
            );
        }
        
        player_settings.counter++;
    }
}

void reset(FugueSettings& settings)
{
    for (int i = 0; i < NUMBER_FUGUE_PLAYERS; i++)
    {
        settings.player_settings[i].counter = 0;
    }
}
