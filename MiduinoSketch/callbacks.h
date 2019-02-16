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

void handleNoteOnPlaying(ApplicationData& data, byte channel, byte pitch, byte velocity)
{
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
        data.mfb_503_settings.kill_hats = !data.mfb_503_settings.kill_hats;
        break;
    case BSP_PAD_05:
        data.bass_settings.kill = !data.bass_settings.kill;
        if (data.bass_settings.kill)
        {
            stop_notes(data.bass_settings.storage);
        }
        break;
    case BSP_PAD_06:
        break;
    case BSP_PAD_07:
        break;
    case BSP_PAD_08:
        break;
    case BSP_PAD_09:
        data.mfb_503_settings.drum_fill = true;
        break;
    case BSP_PAD_10:
        data.mfb_503_settings.snare_roll = velocity;
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

void handleNoteOnStopped(ApplicationData& data, byte channel, byte pitch, byte velocity)
{
    switch (pitch)
    {
    case BSP_PAD_01: 
        data.harmony.scale.root = Root::ROOT_C;
        break;
    case BSP_PAD_02:
        data.harmony.scale.root = Root::ROOT_D;
        break;
    case BSP_PAD_03:
        data.harmony.scale.root = Root::ROOT_E;
        break;
    case BSP_PAD_04:
        data.harmony.scale.root = Root::ROOT_F;
        break;
    case BSP_PAD_05:
        data.harmony.scale.root = Root::ROOT_G;
        break;
    case BSP_PAD_06:
        data.harmony.scale.root = Root::ROOT_A;
        break;
    case BSP_PAD_07:
        data.harmony.scale.root = Root::ROOT_B;
        break;
    case BSP_PAD_08: 
        break;
    case BSP_PAD_09:
        data.harmony.scale.root = Root::ROOT_C_SHARP;
        break;
    case BSP_PAD_10:
        data.harmony.scale.root = Root::ROOT_D_SHARP;
        break;
    case BSP_PAD_11: 
        break;
    case BSP_PAD_12:
        data.harmony.scale.root = Root::ROOT_F_SHARP;
        break;
    case BSP_PAD_13:
        data.harmony.scale.root = Root::ROOT_G_SHARP;
        break;
    case BSP_PAD_14:
        data.harmony.scale.root = Root::ROOT_A_SHARP;
        break;
    case BSP_PAD_15: 
        set_scale(data.harmony.scale, ScaleType::AEOLIAN);
        break;
    case BSP_PAD_16:
        set_scale(data.harmony.scale, ScaleType::IONIAN); 
        break;
    }
}

void handleNoteOn(ApplicationData& data, byte channel, byte pitch, byte velocity)
{
    press_pad(data.ui_state.pad_state, pitch);
    switch (data.time.state)
    {
    case PlayState::Playing:
        handleNoteOnPlaying(data, channel, pitch, velocity);
        break;
    case PlayState::Paused:
    case PlayState::Stopped:
        handleNoteOnStopped(data, channel, pitch, velocity);
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
            data.mfb_503_settings.kill_hats = false;
        }
        break;
    case BSP_PAD_05:
        if (time_since_press(get_pad_state(data.ui_state.pad_state, pitch)) > SHORT_PRESS_TIME)
        {
            data.bass_settings.kill = false;
        }
        break;
    case BSP_PAD_06:
        break;
    case BSP_PAD_07:
        break;
    case BSP_PAD_08:
        break;
    case BSP_PAD_09:
        data.mfb_503_settings.drum_fill = false;
        break;
    case BSP_PAD_10:
        data.mfb_503_settings.snare_roll = false;
        break;
    case BSP_PAD_11:
        break;
    case BSP_PAD_12:
        break;
    case BSP_PAD_13:
        randomize_fugue_player(data.fugue_settings, 0);
        data.bass_settings.style = BassStyle::BassFugue;
        break;
    case BSP_PAD_14:
        randomize_fugue_player(data.fugue_settings, 1);
        data.bass_dub_settings.style = BassDubStyle::DubFugue;
        break;
    case BSP_PAD_15:
        randomize_fugue_player(data.fugue_settings, 2);
        data.mono_settings.style = MonoStyle::MonoFugue;
        break;
    case BSP_PAD_16:
        randomize_fugue_player(data.fugue_settings, 3);
        data.mono_dub_settings.style = MonoDubStyle::MonoDubLead;
        data.mono_dub_settings.settings.style = MonoStyle::MonoFugue;
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
        if (value < 42)
            data.mfb_503_settings.hat_style = HatStyle::HatClosed;
        else if (value < 84)
            data.mfb_503_settings.hat_style = HatStyle::HatBoth;
        else
            data.mfb_503_settings.hat_style = HatStyle::HatOpen;
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
        //if (value < 64)
        //{
        //    data.poly_settings.type = PolyType::PolyLow;
        //}
        //else
        //{
        //    data.poly_settings.type = PolyType::PolyHigh;
        //}
        break;
    case BSP_KNOB_12:
        data.ui_state.poly_pitch_offset = value;
        break;
    case BSP_KNOB_05:
        // TODO: Bass density
        data.fugue_settings.player_settings[0].density = value;
        break;
    case BSP_KNOB_13:
        data.bass_settings.pitch_range = value;
        data.fugue_settings.player_settings[0].manual_pitch_offset = value;
        break;
    case BSP_KNOB_06:
        data.fugue_settings.player_settings[1].density = value;
        break;
    case BSP_KNOB_14:
        data.fugue_settings.player_settings[1].manual_pitch_offset = value;
        break;
    case BSP_KNOB_07:
        data.fugue_settings.player_settings[2].density = value;
        break;
    case BSP_KNOB_15:
        data.mono_settings.variable_pitch_offset = value;
        data.fugue_settings.player_settings[2].manual_pitch_offset = value;
        break;
    case BSP_KNOB_08:
        data.fugue_settings.player_settings[3].density = value;
        break;
    case BSP_KNOB_16:
        data.mono_dub_settings.settings.variable_pitch_offset = value;
        data.fugue_settings.player_settings[3].manual_pitch_offset = value;
        break;

    case BSP_STEP_01:
        if (value == 0)
        {
            randomize_503_sound(data);
            randomize_503_seq(data.mfb_503_settings);
        }
        break;
    case BSP_STEP_02:
        if (value == 0)
        {
            randomize_503_sound(data);
            randomize_503_seq(data.mfb_503_settings);
            randomize_503_kick(data.mfb_503_settings);
        }
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
            //randomize_poly(data);
        }
        break;
    case BSP_STEP_08:
        if (value == 0)
        {
            //randomize_lead(data);
        }
        break;
    case BSP_STEP_09:
        if (value == 0)
        {
            randomize_bass(data.bass_settings);
            switch (randi(2))
            {
            case 0: data.bass_settings.style = BassStyle::BassArpInterval; break;
            case 1: data.bass_settings.style = BassStyle::BassEuclid; break;
            //case 2: data.bass_settings.style = BassStyle::BassLow; break;
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
        if (time_since_release(get_step_state(data.ui_state.step_state, BSP_STEP_12)) > SHORT_PRESS_TIME)
        {
            if (is_pressed(get_step_state(data.ui_state.step_state, BSP_STEP_12)))
            {
                randomize_bass_dub(data.bass_dub_settings);
                data.bass_dub_settings.style = BassDubStyle::DubOctProbability;
            }
            else
            {
                randomize_bass_dub(data.bass_dub_settings);
                data.bass_dub_settings.style = BassDubStyle::DubUnison;
            }
        }
        break;
    case BSP_STEP_12:
        if (time_since_release(get_step_state(data.ui_state.step_state, BSP_STEP_11)) > SHORT_PRESS_TIME)
        {
            if (is_pressed(get_step_state(data.ui_state.step_state, BSP_STEP_11)))
            {
                randomize_bass_dub(data.bass_dub_settings);
                data.bass_dub_settings.style = BassDubStyle::DubOctProbability;
            }
            else
            {
                randomize_bass_dub(data.bass_dub_settings);
                data.bass_dub_settings.style = BassDubStyle::DubOctave;
            }
        }
        if (value == 0)
        {
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
            if (time_since_release(get_step_state(data.ui_state.step_state, BSP_STEP_16)) > SHORT_PRESS_TIME)
            {
                if (is_pressed(get_step_state(data.ui_state.step_state, BSP_STEP_16)))
                {
                    randomize_mono_dub(data.mono_dub_settings);
                    data.mono_dub_settings.style = MonoDubStyle::MonoDubOctave;
                }
                else
                {
                    randomize_mono_dub(data.mono_dub_settings);
                    data.mono_dub_settings.style = MonoDubStyle::MonoDubLead;
                    switch (randi(2))
                    {
                    case 0: data.mono_dub_settings.settings.style = MonoStyle::MonoPolyRhythm; break;
                    case 1: data.mono_dub_settings.settings.style = MonoStyle::MonoLeadPattern; break;
                    }
                }
            }
        }
        break;
    case BSP_STEP_16:
        if (value == 0)
        {
            if (time_since_release(get_step_state(data.ui_state.step_state, BSP_STEP_15)) > SHORT_PRESS_TIME)
            {
                if (is_pressed(get_step_state(data.ui_state.step_state, BSP_STEP_15)))
                {
                    randomize_mono_dub(data.mono_dub_settings);
                    data.mono_dub_settings.style = MonoDubStyle::MonoDubOctave;
                }
                else
                {
                    randomize_mono_dub(data.mono_dub_settings);
                    data.mono_dub_settings.style = MonoDubStyle::MonoDubLead;
                    data.mono_dub_settings.settings.style = MonoStyle::MonoSixteenths;
                }
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
            data.mfb_503_settings.kill_hats = false;
        }
        break;
    case BSP_STEP_02:
        if (value == 0)
        {
            randomize_all(data);
            data.ui_state.kill_low = true;
            data.ui_state.kill_mid = true;
            data.ui_state.kill_perc = true;
            data.mfb_503_settings.kill_hats = true;
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
    all_notes_off(data.mono_settings.storage);
    all_notes_off(data.mono_dub_settings.settings.storage);

    data.time.step = 0;
    data.time.tick = 0;
    data.time.state = PlayState::Stopped;

    reset(data.fugue_settings);
}
