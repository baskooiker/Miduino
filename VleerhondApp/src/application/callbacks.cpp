#pragma once

#include "application/callbacks.h"

#include <memory>

#include "application/ui.h"

namespace Vleerhond
{
    void handleClock(ApplicationData& data)
    {
        if (data.time.state == PlayState::Stopped)
        {
            return;
        }

        if (Utils::intervalHit(TimeDivision::Quarter, data.time))
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

        if (Utils::intervalHit(TimeDivision::Sixteenth, data.time))
        {
            data.updatePedalState();
        }
        data.processActiveNotes();
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
        case BSP_ROT_BTM_01:
            break;
        case BSP_ROT_BTM_02:
            break;
        case BSP_ROT_BTM_03:
            break;
        case BSP_ROT_BTM_04:
            data.minitaur.setVariableDensity(value);
            break;
        case BSP_ROT_BTM_05:
            data.mam_mb33.setVariableDensity(value);
            break;
        case BSP_ROT_BTM_06:
            break;
        case BSP_ROT_BTM_07:
            data.neutron.setVariableDensity(value);
            break;
        case BSP_ROT_BTM_08:
            break;

        case BSP_ROT_TOP_01:
            break;
        case BSP_ROT_TOP_02:
            break;
        case BSP_ROT_TOP_03:
            data.minitaur.setVariableOctave(value);
            break;
        case BSP_ROT_TOP_04:
            data.minitaur.setVariablePitchOffset(value);
            break;
        case BSP_ROT_TOP_05:
            data.mam_mb33.setVariableOctave(value);
            break;
        case BSP_ROT_TOP_06:
            data.mam_mb33.setVariablePitchOffset(value);
            break;
        case BSP_ROT_TOP_07:
            data.neutron.setVariablePitchOffset(value);
            break;
        case BSP_ROT_TOP_08:
            break;

        case BSP_STEP_01:
            if (value == 0)
            {
                if (data.ui_state.is_pressed(BSP_PAD_BTM_08))
                {
                    data.harmony.randomize();
                }
                else
                {
                    data.harmony.setTonic();
                }
            }
            break;
        case BSP_PAD_TOP_01:
            if (value == 0)
            {
                if (data.ui_state.is_pressed(BSP_PAD_BTM_08))
                {
                    data.tanzbar.tanzbar_lo.randomize_alternative();
                }
                else
                {
                    for (auto& inst : data.tanzbar.getLow())
                        inst->randomize();
                }
                data.time.randomizeShuffle();
                data.modulators.randomize();
            }
            break;
        case BSP_STEP_02:
            if (value == 0)
            {
                data.harmony.setType(HarmonyType::Const);
                data.harmony.switchConstChord();
            }
            break;
        case BSP_PAD_TOP_02:
            if (value == 0)
            {
                for (auto& inst : data.tanzbar.getMid())
                    inst->randomize();
                for (auto& inst : data.drumstation.getMid())
                    inst->randomize();

                data.time.randomizeShuffle();
                data.modulators.randomize();
            }
            break;
        case BSP_STEP_03:
            if (value == 0)
            {
                if (data.ui_state.is_pressed(BSP_PAD_BTM_08))
                {
                    data.addEvent(std::make_shared<ChangeHarmonyEvent>(
                        HarmonyType::DominantLow, &data.harmony, &data.time
                        ));
                }
                else
                {
                    data.addEvent(std::make_shared<ChangeHarmonyEvent>(
                        HarmonyType::TonicLow, &data.harmony, &data.time
                        ));
                }
            }
            break;
        case BSP_PAD_TOP_03:
            if (value == 0)
            {
                for (auto& inst : data.tanzbar.getHigh())
                    inst->randomize();
                for (auto& inst : data.drumstation.getHigh())
                    inst->randomize();

                data.time.randomizeShuffle();
                data.modulators.randomize();
            }
            break;
        case BSP_STEP_04:
            if (value == 0)
            {
                if (data.ui_state.is_pressed(BSP_PAD_BTM_08))
                {
                    data.addEvent(std::make_shared<ChangeHarmonyEvent>(
                        HarmonyType::DominantHigh, &data.harmony, &data.time
                        ));
                }
                else
                {
                    data.addEvent(std::make_shared<ChangeHarmonyEvent>(
                        HarmonyType::TonicHigh, &data.harmony, &data.time
                        ));
                }
            }
            break;
        case BSP_PAD_TOP_04:
            if (value == 0)
            {
                data.minitaur.randomize();
            }
            break;
        case BSP_STEP_05:
            if (value == 0)
            {
                data.minitaur.select(0);
            }
            break;
        case BSP_STEP_06:
            if (value == 0)
            {
                data.minitaur.select(1);
            }
            break;
        case BSP_PAD_TOP_05:
        case BSP_PAD_TOP_06:
            if (value == 0)
            {
                data.mam_mb33.randomize();
            }
            break;
        case BSP_STEP_07:
            break;
        case BSP_STEP_08:
            if (value == 0)
            {
            }
            break;
        case BSP_PAD_TOP_07:
            if (value == 0)
            {
                data.neutron.randomize();
            }
            break;
        case BSP_PAD_TOP_08:
            break;
        case BSP_PAD_BTM_01:
            if (data.ui_state.is_pressed(BSP_PAD_BTM_08))
            {
            }
            else
            {
                data.tanzbar.killLow(value > 0);
                data.drumstation.killLow(value > 0);
            }
            break;
        case BSP_PAD_BTM_02:
            data.tanzbar.killMid(value > 0);
            data.drumstation.killMid(value > 0);
            break;
        case BSP_PAD_BTM_03:
            data.tanzbar.killHigh(value > 0);
            data.drumstation.killHigh(value > 0);
            break;
        case BSP_PAD_BTM_04:
            data.minitaur.kill(value > 0);
            break;
        case BSP_PAD_BTM_05:
            data.mam_mb33.kill(value > 0);
            break;
        case BSP_PAD_BTM_06:
            break;
        case BSP_PAD_BTM_07:
            data.neutron.kill(value > 0);
            break;
        case BSP_PAD_BTM_08:
            break;
        case BSP_STEP_09:
            if (value == 0)
            {
                data.mam_mb33.select(0);
            }
            break;
        case BSP_STEP_10:
            if (value == 0)
            {
                data.mam_mb33.select(1);
            }
            break;
        case BSP_STEP_11:
            if (value == 0)
            {
                data.mam_mb33.select(2);
            }
            break;
        case BSP_STEP_12:
            if (value == 0)
            {
                data.mam_mb33.select(3);
            }
            break;
        case BSP_STEP_13:
            if (value == 0)
            {
                data.neutron.select(0);
            }
            break;
        case BSP_STEP_14:
            if (value == 0)
            {
                data.neutron.select(1);
            }
            break;
        case BSP_STEP_15:
            if (value == 0)
            {
                data.neutron.select(2);
            }
            break;
        case BSP_STEP_16:
            if (value == 0)
            {
                data.neutron.select(3);
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
            }
            break;
        case BSP_PAD_TOP_01:
            if (value == 0)
            {
                data.randomize_all();
            }
            break;
        case BSP_STEP_02:
            break;
        case BSP_PAD_TOP_02:
            break;

        case BSP_PAD_BTM_01:
            data.harmony.scale.setRoot(Root::ROOT_C);
            break;
        case BSP_PAD_BTM_02:
            data.harmony.scale.setRoot(Root::ROOT_D);
            break;
        case BSP_PAD_BTM_03:
            data.harmony.scale.setRoot(Root::ROOT_E);
            break;
        case BSP_PAD_BTM_04:
            data.harmony.scale.setRoot(Root::ROOT_F);
            break;
        case BSP_PAD_BTM_05:
            data.harmony.scale.setRoot(Root::ROOT_G);
            break;
        case BSP_PAD_BTM_06:
            data.harmony.scale.setRoot(Root::ROOT_A);
            break;
        case BSP_PAD_BTM_07:
            data.harmony.scale.setRoot(Root::ROOT_B);
            break;
        case BSP_PAD_BTM_08:
            break;
        case BSP_STEP_09:
            data.harmony.scale.setRoot(Root::ROOT_C_SHARP);
            break;
        case BSP_STEP_10:
            data.harmony.scale.setRoot(Root::ROOT_D_SHARP);
            break;
        case BSP_STEP_11:
            break;
        case BSP_STEP_12:
            data.harmony.scale.setRoot(Root::ROOT_F_SHARP);
            break;
        case BSP_STEP_13:
            data.harmony.scale.setRoot(Root::ROOT_G_SHARP);
            break;
        case BSP_STEP_14:
            data.harmony.scale.setRoot(Root::ROOT_A_SHARP);
            break;
        case BSP_STEP_15:
            data.harmony.scale.setScale(ScaleType::AEOLIAN);
            break;
        case BSP_STEP_16:
            data.harmony.scale.setScale(ScaleType::IONIAN);
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