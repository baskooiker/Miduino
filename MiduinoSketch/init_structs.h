#pragma once

#include "defs.h"
#include "scales.h"

TimeStruct init_time()
{
    TimeStruct s = {};

    s.tick = 0;
    s.state = PlayState::Stopped;

    return s;
}

UiState init_ui_state()
{
    UiState s = { 0 };    
    return s;
}

PitchStorage init_pitch_storage()
{
    PitchStorage p = { 0 };
    p.size = 0;
    p.nr_of_events = 0;
    return p;
}

ArpData init_arp_data()
{
    ArpData a = { 0 };

    a.min = 36;
    a.range = 12;
    a.range_count = 3;
    a.counter = 0;
    a.type = ArpType::UP;
    a.range_type = RangeType::Range;
    a.last_note = 0;

    a.arp_notes_length = 0;

    return a;
}

HarmonyStruct init_harmony_struct()
{
    HarmonyStruct s = {0};

    s.const_pattern = init_cv_pattern_ab();
    s.high_pattern = init_cv_pattern_ab();
    s.type = HarmonyType::HarmonyConst;

    s.scale = get_scale(ScaleType::AEOLIAN, ROOT_C);

    return s;
}

FuguePlayerSettings init_fugue_player_settings()
{
    FuguePlayerSettings s = { 0 };

    s.pitch_offset = 36;
    s.length = 4;
    s.type = FuguePlayerType::FugueForward;
    s.interval = NoteInterval::IntervalRoot;
    s.note_repeat = 1;

    return s;
}

FugueSettings init_fugue_settings()
{
    FugueSettings s = { 0 };
    s.pattern = init_cv_pattern();
    for (int i = 0; i < 4; i++)
    {
        s.player_settings[i] = init_fugue_player_settings();
    }
    return s;
}
