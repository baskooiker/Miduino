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

    void handleControlChangePlaying(ApplicationData& data, uint8_t channel, uint8_t number, uint8_t value)
    {
        switch (number)
        {
        case ROTARY_01:
            break;
        case KNOB_01:
            break;
        case ROTARY_02:
            break;
        case KNOB_02:
            break;
        case ROTARY_03:
            break;
        case KNOB_03:
            break;
        case ROTARY_04:
            break;
        case KNOB_04:
            data.rocket_bass.density = value;
            break;
        case ROTARY_05:
            break;
        case KNOB_05:
            data.acid_bass.density = value;
            data.fugue_vermona_1.density = value;
            break;
        case ROTARY_06:
            break;
        case KNOB_06:
            data.fugue_vermona_2.density = value;
            break;
        case ROTARY_07:
            break;
        case KNOB_07:
            data.fugue_vermona_3.density = value;
            data.mono.variable_density = value;
            break;
        case ROTARY_08:
            break;
        case KNOB_08:
            data.fugue_vermona_4.density = value;
            break;

        case SLIDER_01:
            break;
        case SLIDER_02:
            break;
        case SLIDER_03:
            data.rocket_bass.note_range_value = value;
            break;
        case SLIDER_04:
            data.rocket_bass.pitch_range = value;
            break;
        case SLIDER_05:
            data.acid_bass.pitch_range = value;
            data.fugue_vermona_1.manual_pitch_offset = value;
            break;
        case SLIDER_06:
            //data.bass_dub_settings.v_pitch = value;
            data.fugue_vermona_2.manual_pitch_offset = value;
            data.drone.variable_pitch_range = value;
            break;
        case SLIDER_07:
            data.mono.variable_pitch_offset = value;
            data.fugue_vermona_3.manual_pitch_offset = value;
            break;
        case SLIDER_08:
            data.mono_dub.variable_pitch_offset = value;
            data.fugue_vermona_4.manual_pitch_offset = value;
            break;

        case BTN_LEFT_TOP_01:
            if (value == 0)
            {
                data.harmony.randomize();
                data.harmony.type = HarmonyType::HarmonyConst;
                data.harmony.const_value = 0;
            }
            break;
        case BTN_LEFT_BTM_01:
            if (value == 0)
            {
                data.tanzbar_lo.randomize();
                data.time.randomize_shuffle();
                data.modulators.randomize();
            }
            break;
        case BTN_LEFT_TOP_02:
            if (value == 0)
            {
                data.harmony.randomize();
                data.harmony.type = HarmonyType::HarmonyConst;
                data.harmony.switch_const_chord();
            }
            break;
        case BTN_LEFT_BTM_02:
            if (value == 0)
            {
                data.tanzbar_cy.randomize();
                data.tanzbar_ma.randomize();
                data.tanzbar_hats.randomize();
                data.time.randomize_shuffle();
                data.modulators.randomize();
            }
            break;
        case BTN_LEFT_TOP_03:
            if (value == 0)
            {
                data.harmony.randomize();
                data.harmony.type = HarmonyType::HarmonyHigh;
            }
            break;
        case BTN_LEFT_BTM_03:
            if (value == 0)
            {
            }
            break;
        case BTN_LEFT_TOP_04:
            if (value == 0)
            {
            }
            break;
        case BTN_LEFT_BTM_04:
            if (value == 0)
            {
                data.rocket_bass.total_randomize();
            }
            break;
        case BTN_LEFT_TOP_05:
            if (value == 0)
            {
            }
            break;
        case BTN_LEFT_BTM_05:
            if (value == 0)
            {
                data.acid_bass.total_randomize();
            }
            break;
        case BTN_LEFT_TOP_06:
            if (value == 0)
            {
            }
            break;
        case BTN_LEFT_BTM_06:
            if (value == 0)
            {
                data.drone.randomize();
            }
            break;
        case BTN_LEFT_TOP_07:
        case BTN_LEFT_BTM_07:
            if (value == 0)
            {
                handle_step_release(data, number, BTN_LEFT_TOP_07, BTN_LEFT_BTM_07,
                    release_button_left_top_07, release_button_left_btm_07, release_buttons_left_07);
            }
            break;
        case BTN_LEFT_TOP_08:
        case BTN_LEFT_BTM_08:
            if (value == 0)
            {
                handle_step_release(data, number, BTN_LEFT_TOP_08, BTN_LEFT_BTM_08,
                    release_button_left_top_08, release_button_left_btm_08, release_buttons_left_08);
            }
            break;

        case BTN_RIGHT_BTM_01:
            data.tanzbar_lo.kill = value > 0;
            data.mfb_503_kick.kill = value > 0;
            break;
        case BTN_RIGHT_BTM_02:
            break;
        case BTN_RIGHT_BTM_03:
            data.tanzbar_hats.kill = value > 0;
            data.tanzbar_cy.kill = value > 0;
            data.tanzbar_ma.kill = value > 0;

            data.mfb_503_hats.kill = value > 0;
            data.mfb_503_cymbal.kill = value > 0;

            break;
        case BTN_RIGHT_BTM_04:
            data.rocket_bass.kill = value > 0;
            if (data.rocket_bass.kill)
            {
                data.rocket_bass.midi_channel.process_active_notes();
            }
            break;
        case BTN_RIGHT_BTM_05:
            data.acid_bass.kill = value > 0;
            if (data.acid_bass.kill)
            {
                data.acid_bass.midi_channel.process_active_notes();
            }
            break;
        case BTN_RIGHT_BTM_06:
            data.drone.kill = value > 0;
            break;
        case BTN_RIGHT_BTM_07:
            data.mono.kill = value > 0;
            break;
        case BTN_RIGHT_BTM_08:
            data.mono_dub.kill = value > 0;
            break;
        case BTN_RIGHT_TOP_01:
            break;
        case BTN_RIGHT_TOP_02:
            data.tanzbar_mid.snare_roll = value > 0;
            break;
        case BTN_RIGHT_TOP_03:
            break;
        case BTN_RIGHT_TOP_04:
            break;
        case BTN_RIGHT_TOP_05:
            break;
        case BTN_RIGHT_TOP_06:
            break;
        case BTN_RIGHT_TOP_07:
            break;
        case BTN_RIGHT_TOP_08:
            break;
        default:
            break;
        }
    }

    void handleControlChangeStopped(ApplicationData& data, uint8_t channel, uint8_t number, uint8_t value)
    {
        switch (number)
        {
        case BTN_LEFT_TOP_01:
            if (value == 0)
            {
                data.randomize_all();
            }
            break;
        case BTN_LEFT_BTM_01:
            if (value == 0)
            {
                data.randomize_all();
            }
            break;
        case BTN_LEFT_TOP_02:
            if (value == 0)
            {
                data.set_regular();
            }
            break;
        case BTN_LEFT_BTM_02:
            if (value == 0)
            {
                data.set_fugue();
            }
            break;

        case BTN_RIGHT_BTM_01:
            data.harmony.scale.set_root(Root::ROOT_C);
            break;
        case BTN_RIGHT_BTM_02:
            data.harmony.scale.set_root(Root::ROOT_D);
            break;
        case BTN_RIGHT_BTM_03:
            data.harmony.scale.set_root(Root::ROOT_E);
            break;
        case BTN_RIGHT_BTM_04:
            data.harmony.scale.set_root(Root::ROOT_F);
            break;
        case BTN_RIGHT_BTM_05:
            data.harmony.scale.set_root(Root::ROOT_G);
            break;
        case BTN_RIGHT_BTM_06:
            data.harmony.scale.set_root(Root::ROOT_A);
            break;
        case BTN_RIGHT_BTM_07:
            data.harmony.scale.set_root(Root::ROOT_B);
            break;
        case BTN_RIGHT_BTM_08:
            break;
        case BTN_RIGHT_TOP_01:
            data.harmony.scale.set_root(Root::ROOT_C_SHARP);
            break;
        case BTN_RIGHT_TOP_02:
            data.harmony.scale.set_root(Root::ROOT_D_SHARP);
            break;
        case BTN_RIGHT_TOP_03:
            break;
        case BTN_RIGHT_TOP_04:
            data.harmony.scale.set_root(Root::ROOT_F_SHARP);
            break;
        case BTN_RIGHT_TOP_05:
            data.harmony.scale.set_root(Root::ROOT_G_SHARP);
            break;
        case BTN_RIGHT_TOP_06:
            data.harmony.scale.set_root(Root::ROOT_A_SHARP);
            break;
        case BTN_RIGHT_TOP_07:
            data.harmony.scale.set_scale(ScaleType::AEOLIAN);
            break;
        case BTN_RIGHT_TOP_08:
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