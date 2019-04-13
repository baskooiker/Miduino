#pragma once

#include "defs.h"

#include "application_data.h"
#include "chords.h"
#include "defs.h"
#include "midi_io.h"
#include "rand.h"
#include "step_callbacks.h"
#include "ui.h"

#include "lead.h"
#include "mfb_503.h"
#include "mfb_522.h"
#include "poly.h"

void handleNoteOnPlaying(ApplicationData& data, uint8_t channel, uint8_t pitch, uint8_t velocity)
{
    switch (pitch)
    {
    case BSP_PAD_01:
        data.tanzbar_lo.kill = !data.tanzbar_lo.kill;
        break;
    case BSP_PAD_02:
        data.tanzbar_mid.kill = !data.tanzbar_mid.kill;
        break;
    case BSP_PAD_03:
        data.tanzbar_perc.kill = !data.tanzbar_perc.kill;
        break;
    case BSP_PAD_04:
        data.tanzbar_hi.kill = !data.tanzbar_hi.kill;
        break;
    case BSP_PAD_05:
        data.bass_settings.kill = !data.bass_settings.kill;
        if (data.bass_settings.kill)
        {
            data.bass_settings.storage.process_active_notes();
        }
        break;
    case BSP_PAD_06:
        data.bass_dub_settings.kill = !data.bass_dub_settings.kill;
        data.drone.kill = !data.drone.kill;
        break;
    case BSP_PAD_07:
        data.mono_settings.kill = !data.mono_settings.kill;
        break;
    case BSP_PAD_08:
        data.mono_dub_settings.kill = !data.mono_dub_settings.kill;
        break;
    case BSP_PAD_09:
        break;
    case BSP_PAD_10:
        data.tanzbar_mid.snare_roll = velocity;
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

void handleNoteOnStopped(ApplicationData& data, uint8_t channel, uint8_t pitch, uint8_t velocity)
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
        data.harmony.scale.set_scale(ScaleType::AEOLIAN);
        break;
    case BSP_PAD_16:
        data.harmony.scale.set_scale(ScaleType::IONIAN);
        break;
    }
}

void handleNoteOn(ApplicationData& data, uint8_t channel, uint8_t pitch, uint8_t velocity)
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

void handleNoteOff(ApplicationData& data, uint8_t channel, uint8_t pitch, uint8_t velocity)
{
    release_pad(data.ui_state.pad_state, pitch);
    switch (pitch)
    {
    case BSP_PAD_01:
        if (time_since_press(get_pad_state(data.ui_state.pad_state, pitch)) > SHORT_PRESS_TIME)
        {
            data.tanzbar_lo.kill = false;
        }
    break;
    case BSP_PAD_02:
        if (time_since_press(get_pad_state(data.ui_state.pad_state, pitch)) > SHORT_PRESS_TIME)
        {
            data.tanzbar_mid.kill = false;
        }
    break;
    case BSP_PAD_03:
        if (time_since_press(get_pad_state(data.ui_state.pad_state, pitch)) > SHORT_PRESS_TIME)
        {
            data.tanzbar_perc.kill = false;
        }
        break;
    case BSP_PAD_04:
        if (time_since_press(get_pad_state(data.ui_state.pad_state, pitch)) > SHORT_PRESS_TIME)
        {
            data.tanzbar_hi.kill = false;
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
        break;
    case BSP_PAD_10:
        data.tanzbar_mid.snare_roll = false;
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
    release_pad(data.ui_state.pad_state, pitch);
}

void handleClock(ApplicationData& data)
{
    data.time.state = PlayState::Playing;

    if (Utils::interval_hit(TimeDivision::Quarter, data.time))
    {
        uint32_t now = millis();
        if (data.time.last_pulse_time > 0)
        {
            float time_diff = (float)(now - data.time.last_pulse_time);
            if (data.time.average_pulse_time < .1)
            {
                data.time.average_pulse_time = time_diff;
            }
            else
            {
                data.time.average_pulse_time = .9f * data.time.average_pulse_time +
                    .1f * time_diff;
            }
        }
        data.time.last_pulse_time = now;
    }

    data.process_active_notes();
    data.play_all();

    data.time.tick += 1;
}

void handle_step_release(
    ApplicationData& data,
    uint8_t released_button,
    uint8_t button_1,
    uint8_t button_2,
    void callback_one(ApplicationData&),
    void callback_two(ApplicationData&),
    void callback_both(ApplicationData&))
{
    ButtonState other_button = released_button == button_1 ?
        get_step_state(data.ui_state.step_state, button_2) :
        get_step_state(data.ui_state.step_state, button_1);

    if (time_since_release(other_button) > SHORT_PRESS_TIME)
    {
        if (is_pressed(other_button))
        {
            callback_both(data);
        }
        else
        {   
            if (released_button == button_1)
                callback_one(data);
            else if (released_button == button_2)
                callback_two(data);
        }
    }
}

void handleControlChangePlaying(ApplicationData& data, uint8_t channel, uint8_t number, uint8_t value)
{
    switch (number)
    {
    case BSP_KNOB_01:
        break;
    case BSP_KNOB_09:
        break;
    case BSP_KNOB_02:
        break;
    case BSP_KNOB_10:
        break;
    case BSP_KNOB_03:
        break;
    case BSP_KNOB_11:
        break;
    case BSP_KNOB_04:
        break;
    case BSP_KNOB_12:
        data.bass_settings.note_range_value = value;
        break;
    case BSP_KNOB_05:
        data.bass_settings.density = value;
        data.fugue_settings.player_settings[0].density = value;
        break;
    case BSP_KNOB_13:
        data.bass_settings.pitch_range = value;
        data.fugue_settings.player_settings[0].manual_pitch_offset = value;
        break;
    case BSP_KNOB_06:
        data.bass_dub_settings.density = value;
        data.fugue_settings.player_settings[1].density = value;
        break;
    case BSP_KNOB_14:
        data.bass_dub_settings.v_pitch = value;
        data.fugue_settings.player_settings[1].manual_pitch_offset = value;
        data.drone.variable_pitch_range = value;
        break;
    case BSP_KNOB_07:
        data.fugue_settings.player_settings[2].density = value;
        data.mono_settings.variable_density = value;
        break;
    case BSP_KNOB_15:
        data.mono_settings.variable_pitch_offset = value;
        data.fugue_settings.player_settings[2].manual_pitch_offset = value;
        break;
    case BSP_KNOB_08:
        data.fugue_settings.player_settings[3].density = value;
        data.mono_dub_settings.variable_density = value;
        break;
    case BSP_KNOB_16:
        data.mono_dub_settings.variable_pitch_offset = value;
        data.mono_dub_settings.variable_pitch_offset = value;
        data.fugue_settings.player_settings[3].manual_pitch_offset = value;
        break;

    case BSP_STEP_01:
    case BSP_STEP_02:
        if (value == 0)
        {
            handle_step_release(data, number, BSP_STEP_01, BSP_STEP_02,
                release_step_1, release_step_2, release_step_1_2);
        }
        break;
    case BSP_STEP_03:
    case BSP_STEP_04:
        if (value == 0)
        {
            handle_step_release(data, number, BSP_STEP_03, BSP_STEP_04,
                release_step_3, release_step_4, release_step_3_4);
        }
        break;
    case BSP_STEP_05:
    case BSP_STEP_06:
        if (value == 0)
        {
            handle_step_release(data, number, BSP_STEP_05, BSP_STEP_06,
                release_step_5, release_step_6, release_step_5_and_6);
        }
        break;
    case BSP_STEP_07:
        if (value == 0)
        {
            release_step_7(data);
        }
        break;
    case BSP_STEP_08:
        if (value == 0)
        {
            release_step_8(data);
        }
        break;
    case BSP_STEP_09:
        if (value == 0)
        {
            release_step_9(data);
        }
        break;
    case BSP_STEP_10:
        if (value == 0)
        {
            release_step_10(data);
        }
        break;
    case BSP_STEP_11:
    case BSP_STEP_12:
        if (value == 0)
        {
            handle_step_release(data, number, BSP_STEP_11, BSP_STEP_12,
                release_step_11, release_step_12, release_step_11_and_12);
        }
        break;
    case BSP_STEP_13:
    case BSP_STEP_14:
        if (value == 0)
        {
            handle_step_release(data, number, BSP_STEP_13, BSP_STEP_14,
                release_step_13, release_step_14, release_step_13_and_14);
        }
        break;
    case BSP_STEP_15:
    case BSP_STEP_16:
        if (value == 0)
        {
            handle_step_release(data, number, BSP_STEP_15, BSP_STEP_16,
                release_step_15, release_step_16, release_step_15_and_16);
        }
        break;
    default:
        break;
    }
}

void handleControlChangeStopped(ApplicationData& data, uint8_t channel, uint8_t number, uint8_t value)
{
    switch (number)
    {
    case BSP_STEP_01:
        if (value == 0)
        {
            data.randomize_all();
            data.tanzbar_lo.kill = false;
            data.tanzbar_mid.kill = false;
            data.tanzbar_perc.kill = false;
            data.tanzbar_hi.kill = false;
        }
        break;
    case BSP_STEP_02:
        if (value == 0)
        {
            data.randomize_all();
            data.tanzbar_lo.kill = true;
            data.tanzbar_mid.kill = true;
            data.tanzbar_perc.kill = true;
            data.tanzbar_hi.kill = true;
        }
        break;
    case BSP_STEP_16:
        if (value == 0)
        {
            data.tanzbar_lo.randomize();
            data.tanzbar_mid.randomize();
            data.tanzbar_perc.randomize();
            data.tanzbar_hi.randomize();
            data.set_fugue();
        }
        break;
    }
}

void handleControlChange(ApplicationData& data, uint8_t channel, uint8_t number, uint8_t value)
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
    data.stop_all();

    data.time.reset();

    data.fugue_settings.reset();
}
