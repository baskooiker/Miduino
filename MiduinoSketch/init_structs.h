#pragma once

#include "defs.h"

UiState init_ui_state()
{
    UiState s = { 0 };

    s.control_mode = ControlMode::CONTROL_MODE_NORMAL;

    s.kill_bass = false;
    s.kill_high = false;
    s.kill_perc = false;
    s.kill_mid = false;
    s.kill_low = false;

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

    return a;
}

HarmonyStruct init_harmony_struct()
{
    HarmonyStruct s = {0};

    s.const_pattern = init_cv_pattern_ab();
    s.low_pattern = init_cv_pattern_ab();
    s.high_pattern = init_cv_pattern_ab();
    s.type = HarmonyType::Const;

    return s;
}
