#pragma once

#include "init_patterns.h"
#include "init_structs.h"

#include "scales.h"

Mfb503Settings init_503_settings()
{
    Mfb503Settings s = { 0 };

    s.bd_pattern = init_gate_pattern_ab();
    s.sd_pattern = init_gate_pattern_ab();
    s.cy_pattern = init_gate_pattern_ab();
    s.tom_pattern = init_cv_pattern_ab();
    s.tom_mask = init_gate_pattern_ab();

    s.hh_pattern = init_gate_pattern_ab();
    s.oh_pattern = init_gate_pattern_ab();
    s.hat_int_pattern = init_interval_pattern();
    s.hat_velocity = init_cv_pattern_ab();
    s.hat_style = HatStyle::HatClosed;
    s.hat_closed_style = HatClosedStyle::HatClosedRegular;
    s.volume_cy = 0;
    s.volume_tom = 0;
    s.kill_hats = false;
    s.closed_hat_note = NOTE_503_HH_1;

    s.storage = init_pitch_storage();

    return s;
}

TanzbarSettings init_tanzbar_settings()
{
    TanzbarSettings s = { 0 };

    s.bd_pattern = init_gate_pattern_ab();
    s.sd_pattern = init_gate_pattern_ab();
    s.cy_pattern = init_gate_pattern_ab();
    s.tom_pattern = init_cv_pattern_ab();
    s.tom_mask = init_gate_pattern_ab();

    s.hh_pattern = init_gate_pattern_ab();
    s.oh_pattern = init_gate_pattern_ab();
    s.hat_int_pattern = init_interval_pattern();
    s.hat_velocity = init_cv_pattern_ab();
    s.hat_style = HatStyle::HatClosed;
    s.hat_closed_style = HatClosedStyle::HatClosedRegular;
    s.volume_cy = 0;
    s.volume_tom = 0;
    s.kill_hats = false;
    s.closed_hat_note = NOTE_503_HH_1;

    s.storage = init_pitch_storage();

    return s;
}

Mfb522Settings init_522_settings()
{
    Mfb522Settings s = { 0 };

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

LeadSettings init_lead_settings()
{
    LeadSettings s = { 0 };

    s.arp_data = init_arp_data();
    s.min_pitch_pattern = init_cv_pattern_ab();
    s.pattern_slow = init_gate_pattern_ab();
    s.style = LeadStyle::LeadWhole;
    s.storage = init_pitch_storage();

    return s;
}

MonoSettings init_mono_settings()
{
    MonoSettings s = { };

    s.style = MonoStyle::MonoSixteenths;
    s.arp_data = init_arp_data();
    s.int_pattern = init_interval_pattern();
    s.euclid_pattern = init_gate_pattern_ab();
    s.lead_pattern = init_interval_pattern();
    s.storage = init_pitch_storage();
    s.fugue_id = 0;

    return s;
}

MonoDubSettings init_mono_dub_settings()
{
    MonoDubSettings s = { };

    s.settings = init_mono_settings();
    s.style = MonoDubStyle::MonoDubLead;

    return s;
}

PolySettings init_poly_settings()
{
    PolySettings s = { 0 };

    s.gates_low = init_gate_pattern_ab();
    s.gates = init_gate_pattern_ab();
    s.tie_pattern = init_gate_pattern_ab();
    s.pitch_offset = 48;
    s.type = PolyType::PolyLow;
    s.storage = init_pitch_storage();

    return s;
}

BassSettings init_bass_settings()
{
    BassSettings s = { 0 };

    s.accents = init_gate_pattern_ab();
    s.pitches = init_cv_pattern_ab();
    s.octaves = init_cv_pattern_ab();
    s.variable_octaves = init_cv_pattern_ab();
    s.slides = init_gate_pattern_ab();
    s.probs = init_cv_pattern_ab();
    s.int_pattern = init_interval_pattern();
    s.euclid_pattern = init_gate_pattern_ab();
    s.low_pattern = init_gate_pattern_ab();

    s.pitch_range = 0;
    s.style = BassStyle::BassLow;
    s.note_range_prob = init_cv_pattern_ab();
    s.note_range_value = 0;
    s.density = 0;
    s.octave_offset = 2;
    s.kill = false;

    s.storage = init_pitch_storage();

    return s;
}

BassDubSettings init_bass_dub_settings()
{
    BassDubSettings s = { };

    s.style = BassDubStyle::DubOctave;
    s.note_interval = NoteInterval::IntervalRoot;
    s.octave_probs = init_gate_pattern_ab();
    s.hit_probs = init_gate_pattern_ab();
    s.density = 0;
    s.v_pitch = 0;
    s.storage = init_pitch_storage();

    return s;
}

ApplicationData init_application_data()
{
    ApplicationData i = { 0 };

    i.harmony = init_harmony_struct();

    i.time = init_time();

    i.ui_state = init_ui_state();

    i.fugue_settings = init_fugue_settings();

    i.tanzbar_settings = init_tanzbar_settings();

    i.bass_settings = init_bass_settings();
    i.bass_dub_settings = init_bass_dub_settings();
    i.mono_settings = init_mono_settings();
    i.mono_dub_settings = init_mono_dub_settings();

    i.tanzbar_settings.storage.channel = MIDI_CHANNEL_TANZBAR;
    i.mono_settings.storage.channel = MIDI_CHANNEL_MONO;
    i.mono_dub_settings.settings.storage.channel = MIDI_CHANNEL_MONO_2;
    i.bass_settings.storage.channel = MIDI_CHANNEL_BASS;
    i.bass_dub_settings.storage.channel = MIDI_CHANNEL_BASS_DUB;

    i.bass_settings.fugue_id = 0;
    i.bass_dub_settings.fugue_id = 1;
    i.mono_settings.fugue_id = 2;
    i.mono_dub_settings.settings.fugue_id = 3;

    return i;
}
