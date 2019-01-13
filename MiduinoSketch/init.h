#pragma once

#include "init_patterns.h"
#include "init_structs.h"

#include "scales.h"

Settings503 init_503_settings()
{
    Settings503 s = { 0 };

    s.ac_pattern = init_gate_pattern();
    s.bd_pattern = init_gate_pattern_ab();
    s.sd_pattern = init_gate_pattern_ab();
    s.hh_pattern = init_gate_pattern_ab();
    s.oh_pattern = init_gate_pattern_ab();

    s.storage = init_pitch_storage();

    return s;
}

Settings522 init_522_settings()
{
    Settings522 s = { 0 };

    s.ac_522_pattern = init_gate_pattern();
    s.bd_522_pattern = init_gate_pattern_ab();
    s.lo_tom_522_pattern = init_gate_pattern();
    s.mi_tom_522_pattern = init_gate_pattern();
    s.rs_522_pattern = init_gate_pattern();
    s.clave_522_pattern = init_gate_pattern_ab();
    s.clap_522_pattern = init_gate_pattern_ab();
    s.hh_522_pattern = init_gate_pattern_ab();
    s.oh_522_pattern = init_gate_pattern_ab();
    s.cy_522_pattern = init_gate_pattern_ab();
    s.sd_522_pattern = init_gate_pattern_ab();
    s.use_hh_int = false;
    s.hh_int_pattern = init_interval_pattern();
    s.storage = init_pitch_storage();

    return s;
}

SettingsLead init_lead_settings()
{
    SettingsLead s = { 0 };

    s.arp_data = init_arp_data();
    s.int_pattern;
    s.min_pitch_pattern;
    s.style;
    s.storage;

    return s;
}

SettingsMono init_mono_settings()
{
    SettingsMono s = { 0 };

    s.arp_data = init_arp_data();
    s.int_pattern = init_interval_pattern();
    s.storage = init_pitch_storage();

    return s;
}

SettingsP50 init_p50_settings()
{
    SettingsP50 s = { 0 };

    s.gates_low = init_gate_pattern_ab();
    s.gates = init_gate_pattern_ab();
    s.octave = 4;
    s.type = PolyType::PolyOff;
    s.storage = init_pitch_storage();

    return s;
}

SettingsRocket init_rocket_settings()
{
    SettingsRocket s = { 0 };

    s.accents = init_gate_pattern_ab();
    s.pitches = init_cv_pattern_ab();
    s.octaves = init_cv_pattern_ab();
    s.variable_octaves = init_cv_pattern_ab();
    s.slides = init_gate_pattern_ab();
    s.probs = init_cv_pattern_ab();
    s.int_pattern = init_interval_pattern();
    s.euclid_pattern = init_gate_pattern_ab();
    s.gate_density = 0;
    s.low_velocity = 64;
    s.high_velocity = 100;
    s.pitch_range = 0;
    s.style = RocketStyle::RocketWhole;
    s.note_range_prob = init_cv_pattern_ab();
    s.note_range_value = 0;
    s.storage = init_pitch_storage();

    return s;
}

ApplicationData init_application_data()
{
    ApplicationData i = { 0 };

    i.harmony = init_cv_pattern_ab();
    i.scale = get_scale(AEOLIAN, ROOT_C);
    i.step = 0;
    i.ticks = 0;
    i.uiState = init_ui_state();

    i.settings_503 = init_503_settings();
    i.settings_522 = init_522_settings();
    i.settings_lead = init_lead_settings();
    i.settings_mono = init_mono_settings();
    i.settings_p50 = init_p50_settings();
    i.settings_rocket = init_rocket_settings();

    return i;
}