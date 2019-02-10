#pragma once

#include "defs.h"

#include "app_utils.h"
#include "chords.h"
#include "cv.h"
#include "defs.h"
#include "harmony.h"
#include "init.h"
#include "midi_io.h"
#include "rand.h"
#include "rhythms.h"
#include "scales.h"
#include "ui.h"

#include "lead.h"
#include "mfb_503.h"
#include "mfb_522.h"
#include "mono.h"
#include "mono_dub.h"
#include "poly.h"
#include "bass.h"

void handleNoteOn(ApplicationData& data, byte channel, byte pitch, byte velocity)
{
    press_pad(data.ui_state.pad_state, pitch);
    switch (pitch)
    {
    case BSP_PAD_01:
        data.ui_state.kill_low = !data.ui_state.kill_low;
        break;
    case BSP_PAD_02:
        data.ui_state.kill_mid = !data.ui_state.kill_mid;
        break;
    case BSP_PAD_03:
        data.ui_state.kill_perc = !data.ui_state.kill_perc;
        break;
    case BSP_PAD_04:
        data.ui_state.kill_high = !data.ui_state.kill_high;
        break;
    case BSP_PAD_05:
        break;
    case BSP_PAD_06:
        break;
    case BSP_PAD_07:
        break;
    case BSP_PAD_08:
        data.bass_settings.kill = !data.bass_settings.kill;
        if (data.bass_settings.kill)
        {
            stop_notes(data.bass_settings.storage);
        }
        break;
    case BSP_PAD_09:
        data.ui_state.drum_fill = true;
        break;
    case BSP_PAD_10:
        data.ui_state.drum_roll = velocity;
        break;
    case BSP_PAD_11:
        break;
    case BSP_PAD_12:
        break;
    case BSP_PAD_13:
        break;
    case BSP_PAD_14:
        break;
    case BSP_PAD_15:
        break;
    case BSP_PAD_16:
        break;
    default:
        break;
    }
}

void handleNoteOff(ApplicationData& data, byte channel, byte pitch, byte velocity)
{
    release_pad(data.ui_state.pad_state, pitch);
    switch (pitch)
    {
    case BSP_PAD_01:
        if (time_since_press(get_pad_state(data.ui_state.pad_state, pitch)) > SHORT_PRESS_TIME)
        {
            data.ui_state.kill_low = false;
        }
    break;
    case BSP_PAD_02:
        if (time_since_press(get_pad_state(data.ui_state.pad_state, pitch)) > SHORT_PRESS_TIME)
        {
            data.ui_state.kill_mid = false;
        }
    break;
    case BSP_PAD_03:
        if (time_since_press(get_pad_state(data.ui_state.pad_state, pitch)) > SHORT_PRESS_TIME)
        {
            data.ui_state.kill_perc = false;
        }
        break;
    case BSP_PAD_04:
        if (time_since_press(get_pad_state(data.ui_state.pad_state, pitch)) > SHORT_PRESS_TIME)
        {
            data.ui_state.kill_high = false;
        }
        break;
    case BSP_PAD_05:
        break;
    case BSP_PAD_06:
        break;
    case BSP_PAD_07:
        break;
    case BSP_PAD_08:
        if (time_since_press(get_pad_state(data.ui_state.pad_state, pitch)) > SHORT_PRESS_TIME)
        {
            data.bass_settings.kill = false;
        }
        break;
    case BSP_PAD_09:
        data.ui_state.drum_fill = false;
        break;
    case BSP_PAD_10:
        data.ui_state.drum_roll = false;
        break;
    case BSP_PAD_11:
        break;
    case BSP_PAD_12:
        break;
    case BSP_PAD_13:
        randomize_fugue_player(data.fugue_settings, 3);
        break;
    case BSP_PAD_14:
        randomize_fugue_player(data.fugue_settings, 2);
        break;
    case BSP_PAD_15:
        randomize_fugue_player(data.fugue_settings, 1);
        break;
    case BSP_PAD_16:
        randomize_fugue_player(data.fugue_settings, 0);
        break;
    default:
        break;
    }
    release_pad(data.ui_state.pad_state, pitch);
}

void handleClock(ApplicationData& data)
{
    data.time.state = PlayState::Playing;
    stop_notes_all_instruments(data);
    play_all(data);

    data.time.tick += 1;
    if (data.time.tick >= TICKS_PER_STEP)
    {
        data.time.tick = 0;
        data.time.step = (data.time.step + 1) % COMMON_DENOMINATOR;
    }
}

void handleControlChangePlaying(ApplicationData& data, byte channel, byte number, byte value)
{
    switch (number)
    {
    case BSP_KNOB_01:
        data.ui_state.bd_decay_factor = value;
        send_bd_decay(data);
        break;
    case BSP_KNOB_09:
        data.mfb_503_settings.volume_tom = value;
        send_cc(MFB_503_LT_LEVEL, value, MIDI_CHANNEL_503);
        send_cc(MFB_503_MT_LEVEL, value, MIDI_CHANNEL_503);
        send_cc(MFB_503_HT_LEVEL, value, MIDI_CHANNEL_503);
        break;
    case BSP_KNOB_02:
        if (value < 64)
            data.mfb_503_settings.hat_style = HatStyle::HatOffBeat;
        else
            data.mfb_503_settings.hat_style = HatStyle::HatFull;
        break;
    case BSP_KNOB_10:
        data.mfb_503_settings.volume_cy = (value + 1) / 2;
        send_cc(MFB_503_CY_LEVEL, data.mfb_503_settings.volume_cy, MIDI_CHANNEL_503);
        break;
    case BSP_KNOB_03:
        if (value < 10)
            data.harmony.type = HarmonyType::HarmonyConst;
        else if (value < 117)
            data.harmony.type = HarmonyType::HarmonyLow;
        else
            data.harmony.type = HarmonyType::HarmonyHigh;
        break;
    case BSP_KNOB_11:
        break;
    case BSP_KNOB_04:
        if (value < 64)
        {
            data.poly_settings.type = PolyType::PolyLow;
        }
        else
        {
            data.poly_settings.type = PolyType::PolyHigh;
        }
        break;
    case BSP_KNOB_12:
        data.ui_state.poly_pitch_offset = value;
        break;
    case BSP_KNOB_05:
        // TODO: Bass density
        break;
    case BSP_KNOB_13:
        data.bass_settings.pitch_range = value;
        data.fugue_settings.player_settings[0].manual_pitch_offset = value;
        break;
    case BSP_KNOB_06:
        break;
    case BSP_KNOB_14:
        data.fugue_settings.player_settings[1].manual_pitch_offset = value;
        break;
    case BSP_KNOB_07:
        break;
    case BSP_KNOB_15:
        data.mono_settings.variable_pitch_offset = value;
        data.fugue_settings.player_settings[2].manual_pitch_offset = value;
        break;
    case BSP_KNOB_08:
        break;
    case BSP_KNOB_16:
        data.mono_dub_settings.settings.variable_pitch_offset = value;
        data.fugue_settings.player_settings[3].manual_pitch_offset = value;
        break;

    case BSP_STEP_01:
        if (value == 0)
        {
            randomize_503_sound(data);
            randomize_503_seq(data);
        }
        break;
    case BSP_STEP_02:
        break;
    case BSP_STEP_03:
        break;
    case BSP_STEP_05:
        if (value == 0)
        {
            randomize_harmony(data);
        }
        break;
    case BSP_STEP_07:
        if (value == 0)
        {
            randomize_poly(data);
        }
        break;
    case BSP_STEP_08:
        if (value == 0)
        {
            randomize_lead(data);
        }
        break;
    case BSP_STEP_09:
        if (value == 0)
        {
            randomize_bass(data.bass_settings);
            switch (randi(3))
            {
            case 0: data.bass_settings.style = BassStyle::BassLow; break;
            case 1: data.bass_settings.style = BassStyle::BassArpInterval; break;
            case 2: data.bass_settings.style = BassStyle::BassEuclid; break;
            }
        }
        break;
    case BSP_STEP_10:
        if (value == 0)
        {
            randomize_bass(data.bass_settings);
            data.bass_settings.style = BassStyle::BassSixteenths; 
        }
        break;
    case BSP_STEP_11:
        if (value == 0)
        {
            randomize_bass_dub(data.bass_dub_settings);
        }
        break;
    case BSP_STEP_12:
        if (value == 0)
        {
            randomize_bass_dub(data.bass_dub_settings);
        }
        break;
    case BSP_STEP_13:
        if (value == 0)
        {
            randomize_mono(data.mono_settings);
            switch (randi(2))
            {
            case 0: data.mono_settings.style = MonoStyle::MonoPolyRhythm; break;
            case 1: data.mono_settings.style = MonoStyle::MonoLeadPattern; break;
            }
        }
        break;
    case BSP_STEP_14:
        if (value == 0)
        {
            randomize_mono(data.mono_settings);
            data.mono_settings.style = MonoStyle::MonoSixteenths;
        }
        break;
    case BSP_STEP_15:
        if (value == 0)
        {
            randomize_mono_dub(data.mono_dub_settings);
            data.mono_dub_settings.style = MonoDubStyle::MonoDubLead;
        }
        break;
    case BSP_STEP_16:
        if (value == 0)
        {
            randomize_mono_dub(data.mono_dub_settings);
            switch (randi(2))
            {
            case 0: data.mono_dub_settings.style = MonoDubStyle::MonoDubOctave; break;
            case 1: data.mono_dub_settings.style = MonoDubStyle::MonoDubUnison; break;
            }
        }
        break;
    default:
        break;
    }
}

void handleControlChangeStopped(ApplicationData& data, byte channel, byte number, byte value)
{
    switch (number)
    {
    case BSP_STEP_01:
        if (value == 0)
        {
            randomize_all(data);
            data.ui_state.kill_low  = false;
            data.ui_state.kill_mid  = false;
            data.ui_state.kill_perc = false;
            data.ui_state.kill_high = false;
        }
        break;
    case BSP_STEP_02:
        if (value == 0)
        {
            randomize_all(data);
            data.ui_state.kill_low = true;
            data.ui_state.kill_mid = true;
            data.ui_state.kill_perc = true;
            data.ui_state.kill_high = true;
        }
        break;
    case BSP_STEP_16:
        if (value == 0)
        {
            randomize_503_sound(data);
            set_fugue(data);
        }
        break;
    }
}

void handleControlChange(ApplicationData& data, byte channel, byte number, byte value)
{
    if (value > 0)
    {
        press_step(data.ui_state.step_state, number);
    }
    else
    {
        release_step(data.ui_state.step_state, number);
    }
    switch (data.time.state)
    {
    case PlayState::Playing:
        return handleControlChangePlaying(data, channel, number, value);
    case PlayState::Paused:
    case PlayState::Stopped:
        return handleControlChangeStopped(data, channel, number, value);
    }
}

void handleStop(ApplicationData& data)
{
    all_notes_off(data.mfb_503_settings.storage);
    all_notes_off(data.bass_settings.storage);
    all_notes_off(data.bass_dub_settings.storage);
    all_notes_off(data.poly_settings.storage);
    all_notes_off(data.lead_settings.storage);
    all_notes_off(data.mono_settings.storage);
    all_notes_off(data.mono_dub_settings.settings.storage);

    data.time.step = 0;
    data.time.tick = 0;
    data.time.state = PlayState::Stopped;

    reset(data.fugue_settings);
}
