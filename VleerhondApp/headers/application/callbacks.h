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

namespace Vleerhond
{
    void handleClock(ApplicationData& data)
    {
        if (data.time.state == PlayState::Stopped)
        {
            return;
        }

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
                ofLogVerbose("time", "pulse time: %f", time_diff);
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

    void handleControlChangeAlways(ApplicationData& data, uint8_t channel, uint8_t number, uint8_t value)
    {
        switch (number)
        {
        case SLIDER_01:
            data.mfb_503_kick.set_volume(value);
            break;
        case SLIDER_02:
            data.mfb_503_snare.set_volume(value);
            break;
        case SLIDER_03:
            data.mfb_503_toms.set_volume(value);
            break;
        case SLIDER_04:
            data.mfb_503_cymbal.set_volume(value);
            break;
        case SLIDER_05:
            data.mfb_503_hats.set_volume(value);
            break;
        case SLIDER_06:
            break;
        case SLIDER_07:
            break;
        case SLIDER_08:
            break;
        }
    }

    void handleControlChangePlaying(ApplicationData& data, uint8_t channel, uint8_t number, uint8_t value)
    {
        switch (number)
        {
        case BSP_KNOB_01:
            break;
        case BSP_KNOB_09:
            data.mfb_503_kick.set_decay(value);
            break;
        case BSP_KNOB_02:
            break;
        case BSP_KNOB_10:
            data.poly_settings.variable_pitch_offset = value;
            break;
        case BSP_KNOB_03:
            break;
        case BSP_KNOB_11:
            data.rocket_bass.note_range_value = value;
            break;
        case BSP_KNOB_04:
            data.rocket_bass.density = value;
            break;
        case BSP_KNOB_12:
            data.rocket_bass.pitch_range = value;
            break;
        case BSP_KNOB_05:
            data.acid_bass.density = value;
            data.fugue_vermona_1.density = value;
            break;
        case BSP_KNOB_13:
            data.acid_bass.pitch_range = value;
            data.fugue_vermona_1.manual_pitch_offset = value;
            break;
        case BSP_KNOB_06:
            //data.bass_dub_settings.density = value;
            data.fugue_vermona_2.density = value;
            break;
        case BSP_KNOB_14:
            //data.bass_dub_settings.v_pitch = value;
            data.fugue_vermona_2.manual_pitch_offset = value;
            data.drone.variable_pitch_range = value;
            break;
        case BSP_KNOB_07:
            data.fugue_vermona_3.density = value;
            data.mono_settings.variable_density = value;
            break;
        case BSP_KNOB_15:
            data.mono_settings.variable_pitch_offset = value;
            data.fugue_vermona_3.manual_pitch_offset = value;
            break;
        case BSP_KNOB_08:
            data.fugue_vermona_4.density = value;
            data.mono_dub_settings.variable_density = value;
            break;
        case BSP_KNOB_16:
            data.mono_dub_settings.variable_pitch_offset = value;
            data.mono_dub_settings.variable_pitch_offset = value;
            data.fugue_vermona_4.manual_pitch_offset = value;
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

        case BSP_PAD_01:
            data.tanzbar_lo.kill = value > 0;
            data.mfb_503_kick.kill = value > 0;
            break;
        case BSP_PAD_02:
            break;
        case BSP_PAD_03:
            data.tanzbar_hats.kill = value > 0;
            data.tanzbar_cy.kill = value > 0;
            data.tanzbar_ma.kill = value > 0;

            data.mfb_503_hats.kill = value > 0;
            data.mfb_503_cymbal.kill = value > 0;

            break;
        case BSP_PAD_04:
            data.rocket_bass.kill = value > 0;
            if (data.rocket_bass.kill)
            {
                data.rocket_bass.midi_channel.process_active_notes();
            }
            break;
        case BSP_PAD_05:
            data.acid_bass.kill = value > 0;
            if (data.acid_bass.kill)
            {
                data.acid_bass.midi_channel.process_active_notes();
            }
            break;
        case BSP_PAD_06:
            data.drone.kill = value > 0;
            break;
        case BSP_PAD_07:
            data.mono_settings.kill = value > 0;
            break;
        case BSP_PAD_08:
            data.mono_dub_settings.kill = value > 0;
            break;
        case BSP_PAD_09:
            break;
        case BSP_PAD_10:
            data.tanzbar_mid.snare_roll = value > 0;
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

    void handleControlChangeStopped(ApplicationData& data, uint8_t channel, uint8_t number, uint8_t value)
    {
        switch (number)
        {
        case BSP_STEP_01:
            if (value == 0)
            {
                data.randomize_all();
            }
            break;
        case BSP_STEP_02:
            if (value == 0)
            {
                data.randomize_all();
            }
            break;
        case BSP_STEP_03:
            if (value == 0)
            {
                data.set_regular();
            }
            break;
        case BSP_STEP_04:
            if (value == 0)
            {
                data.set_fugue();
            }
            break;
        case BSP_STEP_16:
            break;

        case BSP_PAD_01:
            data.harmony.scale.set_root(Root::ROOT_C);
            break;
        case BSP_PAD_02:
            data.harmony.scale.set_root(Root::ROOT_D);
            break;
        case BSP_PAD_03:
            data.harmony.scale.set_root(Root::ROOT_E);
            break;
        case BSP_PAD_04:
            data.harmony.scale.set_root(Root::ROOT_F);
            break;
        case BSP_PAD_05:
            data.harmony.scale.set_root(Root::ROOT_G);
            break;
        case BSP_PAD_06:
            data.harmony.scale.set_root(Root::ROOT_A);
            break;
        case BSP_PAD_07:
            data.harmony.scale.set_root(Root::ROOT_B);
            break;
        case BSP_PAD_08:
            break;
        case BSP_PAD_09:
            data.harmony.scale.set_root(Root::ROOT_C_SHARP);
            break;
        case BSP_PAD_10:
            data.harmony.scale.set_root(Root::ROOT_D_SHARP);
            break;
        case BSP_PAD_11:
            break;
        case BSP_PAD_12:
            data.harmony.scale.set_root(Root::ROOT_F_SHARP);
            break;
        case BSP_PAD_13:
            data.harmony.scale.set_root(Root::ROOT_G_SHARP);
            break;
        case BSP_PAD_14:
            data.harmony.scale.set_root(Root::ROOT_A_SHARP);
            break;
        case BSP_PAD_15:
            data.harmony.scale.set_scale(ScaleType::AEOLIAN);
            break;
        case BSP_PAD_16:
            data.harmony.scale.set_scale(ScaleType::IONIAN);
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
        handleControlChangeAlways(data, channel, number, value);
        switch (data.time.state)
        {
        case PlayState::Playing:
            return handleControlChangePlaying(data, channel, number, value);
        case PlayState::Stopped:
            return handleControlChangeStopped(data, channel, number, value);
        }
    }

    void handleStop(ApplicationData& data)
    {
        data.stop_all();

        data.time.reset();

        data.fugue_vermona_1.reset();
        data.fugue_vermona_2.reset();
        data.fugue_vermona_3.reset();
        data.fugue_vermona_4.reset();
    }
}