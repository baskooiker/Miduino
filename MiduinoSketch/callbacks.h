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
#include "storage.h"
#include "scales.h"
#include "ui.h"

#include "lead.h"
#include "mfb_503.h"
#include "mfb_522.h"
#include "mono.h"
#include "p50.h"
#include "rocket.h"

void handleNoteOn(ApplicationData& data, byte channel, byte pitch, byte velocity)
{
    switch (pitch)
    {
    case BSP_PAD_01:
        data.uiState.pad_state[0].last_pressed = millis();
        data.uiState.kill_low = !data.uiState.kill_low;
        set_pad_state(data.uiState, 0, true);
        break;
    case BSP_PAD_02:
        data.uiState.pad_state[1].last_pressed = millis();
        data.uiState.kill_mid = !data.uiState.kill_mid;
        set_pad_state(data.uiState, 1, true);
        break;
    case BSP_PAD_03:
        data.uiState.pad_state[2].last_pressed = millis();
        data.uiState.kill_perc = !data.uiState.kill_perc;
        set_pad_state(data.uiState, 2, true);
        break;
    case BSP_PAD_04:
        data.uiState.pad_state[3].last_pressed = millis();
        data.uiState.kill_high = !data.uiState.kill_high;
        set_pad_state(data.uiState, 3, true);
        break;
    case BSP_PAD_05:
        data.uiState.pad_state[4].last_pressed = millis();
        set_pad_state(data.uiState, 4, true);
        break;
    case BSP_PAD_06:
        data.uiState.pad_state[5].last_pressed = millis();
        set_pad_state(data.uiState, 5, true);
        break;
    case BSP_PAD_07:
        data.uiState.pad_state[6].last_pressed = millis();
        set_pad_state(data.uiState, 6, true);
        break;
    case BSP_PAD_08:
        data.uiState.pad_state[7].last_pressed = millis();
        data.uiState.kill_bass = !data.uiState.kill_bass;
        if (data.uiState.kill_bass)
        {
            stop_notes(data.settings_rocket.storage);
        }
        set_pad_state(data.uiState, 7, true);
        break;
    case BSP_PAD_09:
        data.uiState.pad_state[8].last_pressed = millis();
        data.uiState.drum_fill = true;
        set_pad_state(data.uiState, 8, true);
        break;
    case BSP_PAD_10:
        data.uiState.pad_state[9].last_pressed = millis();
        data.uiState.drum_roll = velocity;
        set_pad_state(data.uiState, 9, true);
        break;
    case BSP_PAD_11:
        data.uiState.pad_state[10].last_pressed = millis();
        set_pad_state(data.uiState, 10, true);
        break;
    case BSP_PAD_12:
        data.uiState.pad_state[11].last_pressed = millis();
        set_pad_state(data.uiState, 11, true);
        break;
    case BSP_PAD_13:
        data.uiState.pad_state[12].last_pressed = millis();
        set_pad_state(data.uiState, 12, true);
        break;
    case BSP_PAD_14:
        data.uiState.pad_state[13].last_pressed = millis();
        set_pad_state(data.uiState, 13, true);
        break;
    case BSP_PAD_15:
        data.uiState.pad_state[14].last_pressed = millis();
        set_pad_state(data.uiState, 14, true);
        break;
    case BSP_PAD_16:
        data.uiState.pad_state[15].last_pressed = millis();
        set_pad_state(data.uiState, 15, true);
        break;
    default:
        break;
    }
}

bool was_pressed_long(ButtonState& state)
{
    unsigned long t = millis();
    if (t > state.last_pressed)
    {
        return (t - state.last_pressed) > 500;
    }
    return true;
}

void handleNoteOff(ApplicationData& data, byte channel, byte pitch, byte velocity)
{
    switch (pitch)
    {
    case BSP_PAD_01:
    {
        boolean p_long = was_pressed_long(data.uiState.pad_state[0]);
        if (p_long)
        {
            data.uiState.kill_low = false;
        }
        set_pad_state(data.uiState, 0, false);
    }
    break;
    case BSP_PAD_02:
    {
        boolean p_long = was_pressed_long(data.uiState.pad_state[1]);
        if (p_long)
        {
            data.uiState.kill_mid = false;
        }
        set_pad_state(data.uiState, 1, false);
    }
    break;
    case BSP_PAD_03:
    {
        boolean p_long = was_pressed_long(data.uiState.pad_state[2]);
        if (p_long)
        {
            data.uiState.kill_perc = false;
        }
        set_pad_state(data.uiState, 2, false);
        break;
    }
    case BSP_PAD_04:
    {
        boolean p_long = was_pressed_long(data.uiState.pad_state[3]);
        if (p_long)
        {
            data.uiState.kill_high = false;
        }
        set_pad_state(data.uiState, 3, false);
        break;
    }
    case BSP_PAD_05:
    {
        boolean p_long = was_pressed_long(data.uiState.pad_state[4]);
        set_pad_state(data.uiState, 4, false);
        break;
    }
    case BSP_PAD_06:
    {
        boolean p_long = was_pressed_long(data.uiState.pad_state[5]);
        set_pad_state(data.uiState, 5, false);
        break;
    }
    case BSP_PAD_07:
    {
        boolean p_long = was_pressed_long(data.uiState.pad_state[6]);
        set_pad_state(data.uiState, 6, false);
        break;
    }
    case BSP_PAD_08:
    {
        if (was_pressed_long(data.uiState.pad_state[7]))
        {
            data.uiState.kill_bass = false;
        }
        set_pad_state(data.uiState, 7, false);
        break;
    }
    case BSP_PAD_09:
    {
        boolean p_long = was_pressed_long(data.uiState.pad_state[8]);
        data.uiState.drum_fill = false;
        set_pad_state(data.uiState, 8, false);
        break;
    }
    case BSP_PAD_10:
    {
        data.uiState.drum_roll = false;
        set_pad_state(data.uiState, 9, false);
        break;
    }
    case BSP_PAD_11:
    {
        boolean p_long = was_pressed_long(data.uiState.pad_state[10]);
        set_pad_state(data.uiState, 10, false);
        break;
    }
    case BSP_PAD_12:
    {
        boolean p_long = was_pressed_long(data.uiState.pad_state[11]);
        set_pad_state(data.uiState, 11, false);
        break;
    }
    case BSP_PAD_13:
    {
        boolean p_long = was_pressed_long(data.uiState.pad_state[12]);
        set_pad_state(data.uiState, 12, false);
        break;
    }
    case BSP_PAD_14:
    {
        boolean p_long = was_pressed_long(data.uiState.pad_state[13]);
        set_pad_state(data.uiState, 13, false);
        break;
    }
    case BSP_PAD_15:
    {
        boolean p_long = was_pressed_long(data.uiState.pad_state[14]);
        set_pad_state(data.uiState, 14, false);
        break;
    }
    case BSP_PAD_16:
    {
        boolean p_long = was_pressed_long(data.uiState.pad_state[15]);
        set_pad_state(data.uiState, 15, false);
        break;
    }
    default:
        break;
    }
}

void handleClock(ApplicationData& data)
{
    stop_notes_all_instruments(data);
    play_all(data);

    data.ticks += 1;
    if (data.ticks >= TICKS_PER_STEP)
    {
        data.ticks = 0;
        data.step = (data.step + 1) % COMMON_DENOMINATOR;
    }
}

void handleControlChange(ApplicationData& data, byte channel, byte number, byte value)
{
    switch (number)
    {
    case BSP_KNOB_01:
        data.uiState.bd_decay_factor = value;
        send_bd_decay(data);
        break;
    case BSP_KNOB_02:
        if (value < 64)
            data.settings_503.hat_style = HatStyle::HatOffBeat;
        else
            data.settings_503.hat_style = HatStyle::HatFull;
        break;
    case BSP_KNOB_03:
        if (value < 10)
            data.harmony.type = HarmonyType::Const;
        else if (value < 117)
            data.harmony.type = HarmonyType::Low;
        else
            data.harmony.type = HarmonyType::High;
        break;
    case BSP_KNOB_04:
        if (value < 64)
        {
            data.settings_p50.type = PolyType::PolyLow;
        }
        else 
        {
            data.settings_p50.type = PolyType::PolyHigh;
        }
        break;
    case BSP_KNOB_05:
        break;
    case BSP_KNOB_06:
        break;
    case BSP_KNOB_07:
        data.settings_rocket.pitch_range = value;
        break;
    case BSP_KNOB_08:
        if (value < 10)
            data.settings_rocket.style = RocketLow;
        else if (value < 64)
            data.settings_rocket.style = RocketEuclid;
        else if (value < 120)
            data.settings_rocket.style = RocketArpInterval;
        else
            data.settings_rocket.style = RocketSixteenths;
        break;
    case BSP_KNOB_09:
        data.settings_503.volume_tom = value;
        send_cc(MFB_503_LT_LEVEL, value, MIDI_CHANNEL_503);
        send_cc(MFB_503_MT_LEVEL, value, MIDI_CHANNEL_503);
        send_cc(MFB_503_HT_LEVEL, value, MIDI_CHANNEL_503);
        break;
    case BSP_KNOB_10:
        data.settings_503.volume_cy = (value + 1) / 2;
        send_cc(MFB_503_CY_LEVEL, data.settings_503.volume_cy, MIDI_CHANNEL_503);
        break;
    case BSP_KNOB_12:
        data.uiState.poly_pitch_offset = value;
        break;
    case BSP_KNOB_14:
        data.uiState.mono_pitch_offset = value;
        break;
    case BSP_KNOB_16:
        data.settings_rocket.note_range_value = value;
        break;
    case BSP_STEP_01:
        if (value == 0)
        {
            randomize_503_sound(data);
            randomize_503_seq(data);
            randomize_522_seq(data);
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
            randomize_P50_seq(data);
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
            randomize_mono(data.settings_mono_2);
            data.settings_mono_2.style = MonoStyle::Sixteenths;
        }
        break;
        break;
    case BSP_STEP_10:
        if (value == 0)
        {
            randomize_mono(data.settings_mono_2);
            switch (randi(2))
            {
            case 0: data.settings_mono.style = MonoStyle::PolyRhythm; break;
            case 1: data.settings_mono.style = MonoStyle::LeadPattern; break;
            }
        }
        break;
        break;
    case BSP_STEP_11:
        if (value == 0)
        {
            randomize_mono(data.settings_mono);
            data.settings_mono.style = MonoStyle::Sixteenths; 
        }
        break;
    case BSP_STEP_12:
        if (value == 0)
        {
            randomize_mono(data.settings_mono);
            switch (randi(2))
            {
            case 0: data.settings_mono.style = MonoStyle::PolyRhythm; break;
            case 1: data.settings_mono.style = MonoStyle::LeadPattern; break;
            }
        }
        break;
    case BSP_STEP_13:
        if (value == 0)
        {
            randomize_rocket_seq(data);
        }
        break;
    case BSP_STEP_14:
        break;
    case BSP_STEP_15:
        break;
    case BSP_STEP_16:
        break;
    default:
        break;
    }
}

void handleStop(ApplicationData& data)
{
    all_notes_off(data.settings_503.storage);
    all_notes_off(data.settings_522.storage);
    all_notes_off(data.settings_p50.storage);
    all_notes_off(data.settings_rocket.storage);
    all_notes_off(data.settings_lead.storage);
    all_notes_off(data.settings_mono.storage);
    all_notes_off(data.settings_mono_2.storage);

    data.step = 0;
    data.ticks = 0;

    randomize_all(data);
}
