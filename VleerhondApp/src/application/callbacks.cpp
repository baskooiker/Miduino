#pragma once

#include "application/callbacks.h"
#include "application/ui.h"

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
            uint32_t now = Utils::millis();
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
        ButtonState& other_button = released_button == button_1 ?
            data.ui_state.get_step_state(button_2) :
            data.ui_state.get_step_state(button_1);

        if (other_button.long_since_release())
        {
            if (other_button.is_pressed())
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
            data.moog_bass.note_range_value = value;
            break;
        case ROTARY_04:
            break;
        case KNOB_04:
            data.moog_bass.density = value;
            break;
        case ROTARY_05:
            break;
        case KNOB_05:
            data.tb303_bass.density = value;
            break;
        case ROTARY_06:
            break;
        case KNOB_06:
            break;
        case ROTARY_07:
            break;
        case KNOB_07:
            data.neutron_mono.variable_density = value;
            break;
        case ROTARY_08:
            break;
        case KNOB_08:
            break;

        case SLIDER_01:
            break;
        case SLIDER_02:
            break;
        case SLIDER_03:
            data.moog_bass.variable_octave = value;
            break;
        case SLIDER_04:
            data.moog_bass.pitch_offset = Utils::rerange(value, 24, 24);
            break;
        case SLIDER_05:
            data.tb303_bass.variable_octave = value;
            break;
        case SLIDER_06:
            data.tb303_bass.pitch_offset = Utils::rerange(value, 24, 24);
            break;
        case SLIDER_07:
            data.neutron_mono.variable_pitch_offset = value;
            break;
        case SLIDER_08:
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
                for (auto& inst : data.tanzbar.getLow())
                    inst->randomize();
                for (auto& inst : data.mfb_522.getLow())
                    inst->randomize();

                data.time.randomize_shuffle();
                data.modulators.randomize();
            }
            break;
        case BTN_LEFT_TOP_02:
            if (value == 0)
            {
                data.harmony.type = HarmonyType::HarmonyConst;
                data.harmony.switch_const_chord();
            }
            break;
        case BTN_LEFT_BTM_02:
            if (value == 0)
            {
                for (auto& inst : data.tanzbar.getMid())
                    inst->randomize();
                for (auto& inst : data.mfb_522.getMid())
                    inst->randomize();

                data.time.randomize_shuffle();
                data.modulators.randomize();
            }
            break;
        case BTN_LEFT_TOP_03:
            if (value == 0)
            {
                data.harmony.setHighPattern(0, false);
            }
            break;
        case BTN_LEFT_BTM_03:
            if (value == 0)
            {
                for (auto& inst : data.tanzbar.getHigh())
                    inst->randomize();
                for (auto& inst : data.mfb_522.getHigh())
                    inst->randomize();

                data.time.randomize_shuffle();
                data.modulators.randomize();
            }
            break;
        case BTN_LEFT_TOP_04:
            if (value == 0)
            {
                data.harmony.setHighPattern(0, true);
            }
            break;
        case BTN_LEFT_BTM_04:
            if (value == 0)
            {
                if (data.ui_state.getValue(BSP_SLIDER))
                {

                }
                data.moog_bass.total_randomize();
            }
            break;
        case BTN_LEFT_TOP_05:
            if (value == 0)
            {
                data.harmony.setHighPattern(4, false);
            }
            break;
        case BTN_LEFT_TOP_06:
            if (value == 0)
            {
                data.harmony.setHighPattern(4, true);
            }
            break;
        case BTN_LEFT_BTM_05:
        case BTN_LEFT_BTM_06:
            if (value == 0)
            {
                handle_step_release(data, number, BTN_LEFT_BTM_05, BTN_LEFT_BTM_06,
                    [](ApplicationData& data) 
                    {
                        data.tb303_bass.total_randomize();
                        data.tb303_bass.randomize_drop();
                    },
                    [](ApplicationData& data) 
                    {
                        data.tb303_bass.total_randomize();
                    },
                    [](ApplicationData& data) 
                    {
                        data.tb303_bass.total_randomize();
                        data.tb303_bass.note_range_value = 0;
                    }
                );
                data.tb303_bass.follow_harmony = !data.ui_state.is_pressed(BTN_RIGHT_BTM_08);
            }
            break;
        case BTN_LEFT_TOP_07:
        case BTN_LEFT_TOP_08:
            break;
        case BTN_LEFT_BTM_07:
        case BTN_LEFT_BTM_08:
            if (value == 0)
            {
                handle_step_release(data, number, BTN_LEFT_BTM_07, BTN_LEFT_BTM_08,
                    [](ApplicationData& data)
                    {
                        data.neutron_mono.total_randomize();
                        data.neutron_mono.set_style(MonoStyle::MonoPolyRhythm);
                        data.neutron_mono.set_pitch_mode(MonoPitchMode::SEQUENCE);
                    },
                    [](ApplicationData& data)
                    {
                        data.neutron_mono.total_randomize();
                        data.neutron_mono.set_style(MonoStyle::MonoSixteenths);
                        switch (Rand::distribution(16, 16))
                        {
                        case 0:
                            data.neutron_mono.set_pitch_mode(MonoPitchMode::ARP);
                            break;
                        case 1:
                            data.neutron_mono.set_pitch_mode(MonoPitchMode::SEQUENCE);
                            break;
                        }
                    },
                    [](ApplicationData& data)
                    {
                        if (!data.ui_state.is_pressed(BTN_RIGHT_BTM_08))
                        {
                            data.neutron_mono.set_style(MonoStyle::MonoPolyRhythm);
                            data.neutron_mono.set_pitch_mode(MonoPitchMode::SEQUENCE);
                            data.neutron_mono.set_const_sequence();
                        }
                        else
                        {
                            data.neutron_mono.total_randomize();
                            data.neutron_mono.set_slow_rhythm();
                            data.neutron_mono.set_style(MonoStyle::MonoPolyRhythm);
                            data.neutron_mono.set_arp_type(ArpType::CLOSEST_EXC);
                            data.neutron_mono.set_pitch_mode(MonoPitchMode::ARP);
                            data.neutron_mono.set_arp_range(12);
                        }
                    }
                );
            }
            break;
        case BTN_RIGHT_BTM_01:
            data.tanzbar.killLow(value > 0);
            data.mfb_522.killLow(value > 0);
            break;
        case BTN_RIGHT_BTM_02:
            data.tanzbar.killMid(value > 0);
            data.mfb_522.killMid(value > 0);
            break;
        case BTN_RIGHT_BTM_03:
            data.tanzbar.killHigh(value > 0);
            data.mfb_522.killHigh(value > 0);
            break;
        case BTN_RIGHT_BTM_04:
            data.moog_bass.kill = value > 0;
            if (data.moog_bass.kill)
            {
                data.moog_bass.stop_notes();
                data.moog_bass.midi_channel.process_active_notes();
            }
            break;
        case BTN_RIGHT_BTM_05:
            data.tb303_bass.kill = value > 0;
            if (value > 0)
            {
                data.tb303_bass.midi_channel.process_active_notes();
            }
            break;
        case BTN_RIGHT_BTM_06:
            break;
        case BTN_RIGHT_BTM_07:
            data.neutron_mono.kill = value > 0;
            break;
        case BTN_RIGHT_BTM_08:
            break;
        case BTN_RIGHT_TOP_01:
            break;
        case BTN_RIGHT_TOP_02:
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
            break;
        case BTN_LEFT_BTM_02:
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
            data.ui_state.press_step(number);
        }
        else
        {
            data.ui_state.release_step(number);
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
    }
}