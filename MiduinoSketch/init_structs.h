#pragma once

#include "defs.h"

TimeStruct init_time()
{
    TimeStruct s = {};

    s.step = 0;
    s.tick = 0;
    s.state = PlayState::Stopped;

    return s;
}

UiState init_ui_state()
{
    UiState s = { 0 };

    s.drum_fill = false;
    s.drum_roll = false;

    s.kill_bass = false;
    s.kill_high = false;
    s.kill_perc = false;
    s.kill_mid = false;
    s.kill_low = false;

    s.bd_decay_factor = 0;
    
    return s;
}

PitchStorage init_pitch_storage()
{
    PitchStorage p = { 0 };
    p.size = 0;
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
    s.low_pattern = init_cv_pattern_ab();
    s.high_pattern = init_cv_pattern_ab();
    s.type = HarmonyType::HarmonyConst;

    s.scale = get_scale(AEOLIAN, ROOT_C);

    return s;
}
