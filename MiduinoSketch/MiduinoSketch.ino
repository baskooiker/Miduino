#include <MIDI.h>

#include "defs.h"
#include "rhythms.h"
#include "basslines.h"
#include "storage.h"
#include "scales.h"
#include "ui.h"

#include "mfb_503.h"
#include "mfb_522.h"
#include "p50.h"
#include "rocket.h"

MIDI_CREATE_DEFAULT_INSTANCE();

static ApplicationData data = {};

void handleNoteOn(byte channel, byte pitch, byte velocity)
{
    switch (pitch)
    {
    case BSP_PAD_01:
        if (data.uiState.in_swing_mode)
        {
            data.swing = 0;
        }
        if (data.uiState.in_root_mode)
        {
            data.root = ROOT_C;
        }
        set_pad_state(data.uiState, 0, true);

        break;
    case BSP_PAD_02:
        if (data.uiState.in_swing_mode)
        {
            data.swing = 1;
        }
        if (data.uiState.in_root_mode)
        {
            data.root = ROOT_D;
        }
        set_pad_state(data.uiState, 1, true);
        break;
    case BSP_PAD_03:
        if (data.uiState.in_swing_mode)
        {
            data.swing = 2;
        }
        if (data.uiState.in_root_mode)
        {
            data.root = ROOT_E;
        }
        set_pad_state(data.uiState, 2, true);
        break;
    case BSP_PAD_04:
        if (data.uiState.in_swing_mode)
        {
            data.swing = 3;
        }
        if (data.uiState.in_root_mode)
        {
            data.root = ROOT_F;
        }
        set_pad_state(data.uiState, 3, true);
        break;
    case BSP_PAD_05:
        if (data.uiState.in_root_mode)
        {
            data.root = ROOT_G;
        }
        set_pad_state(data.uiState, 4, true);
        break;
    case BSP_PAD_06:
        if (data.uiState.in_root_mode)
        {
            data.root = ROOT_A;
        }
        set_pad_state(data.uiState, 5, true);
        break;
    case BSP_PAD_07:
        if (data.uiState.in_root_mode)
        {
            data.root = ROOT_B;
        }
        set_pad_state(data.uiState, 6, true);
        break;
    case BSP_PAD_08:
        if (no_button(data.uiState) && no_pad(data.uiState))
        {
            data.uiState.in_swing_mode = true;
        }
        set_pad_state(data.uiState, 7, true);
        break;
    case BSP_PAD_09:
        if (data.uiState.in_root_mode)
        {
            data.root = ROOT_C_SHARP;
        }
        set_pad_state(data.uiState, 8, true);
        break;
    case BSP_PAD_10:
        if (data.uiState.in_root_mode)
        {
            data.root = ROOT_D_SHARP;
        }
        set_pad_state(data.uiState, 9, true);
        break;
    case BSP_PAD_11:
        set_pad_state(data.uiState, 10, true);
        break;
    case BSP_PAD_12:
        if (data.uiState.in_root_mode)
        {
            data.root = ROOT_F_SHARP;
        }
        set_pad_state(data.uiState, 11, true);
        break;
    case BSP_PAD_13:
        if (data.uiState.in_root_mode)
        {
            data.root = ROOT_G_SHARP;
        }
        set_pad_state(data.uiState, 12, true);
        break;
    case BSP_PAD_14:
        if (data.uiState.in_root_mode)
        {
            data.root = ROOT_A_SHARP;
        }
        set_pad_state(data.uiState, 13, true);
        break;
    case BSP_PAD_15:
        set_pad_state(data.uiState, 14, true);
        break;
    case BSP_PAD_16:
        if (no_button(data.uiState) && no_pad(data.uiState))
        {
            data.uiState.in_root_mode = true;
        }
        set_pad_state(data.uiState, 15, true);
        break;
    default:
        break;
    }
}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
    switch (pitch)
    {
    case BSP_PAD_01:
        set_pad_state(data.uiState, 0, false);
        break;
    case BSP_PAD_02:
        set_pad_state(data.uiState, 1, false);
        break;
    case BSP_PAD_03:
        set_pad_state(data.uiState, 2, false);
        break;
    case BSP_PAD_04:
        set_pad_state(data.uiState, 3, false);
        break;
    case BSP_PAD_05:
        set_pad_state(data.uiState, 4, false);
        break;
    case BSP_PAD_06:
        set_pad_state(data.uiState, 5, false);
        break;
    case BSP_PAD_07:
        set_pad_state(data.uiState, 6, false);
        break;
    case BSP_PAD_08:
        data.uiState.in_swing_mode = false;
        set_pad_state(data.uiState, 7, false);
        break;
    case BSP_PAD_09:
        set_pad_state(data.uiState, 8, false);
        break;
    case BSP_PAD_10:
        set_pad_state(data.uiState, 9, false);
        break;
    case BSP_PAD_11:
        set_pad_state(data.uiState, 10, false);
        break;
    case BSP_PAD_12:
        set_pad_state(data.uiState, 11, false);
        break;
    case BSP_PAD_13:
        set_pad_state(data.uiState, 12, false);
        break;
    case BSP_PAD_14:
        set_pad_state(data.uiState, 13, false);
        break;
    case BSP_PAD_15:
        set_pad_state(data.uiState, 14, false);
        break;
    case BSP_PAD_16:
        data.uiState.in_root_mode = false;
        set_pad_state(data.uiState, 15, false);
        break;
    default:
        break;
    }
}

void handleClock()
{
       
    if (data.ticks_counter == (data.step % 2 == 0 ? 0 : data.swing))
    {
        play_all();
    }
    data.ticks_counter += 1;
    if (data.ticks_counter > TICKS_PER_STEP)
    {
        data.ticks_counter = 0;
        data.step = (data.step + 1) % COMMON_DENOMINATOR;
    }
}

void handleControlChange(byte channel, byte number, byte value)
{
    switch (number)
    {
    case BSP_KNOB_08:
        data.rocket_density = (value / 127.) * .8f + .2f;
        break;
    case BSP_STEP_01:
        if (value == 0)
        {
            randomize_503_sound();
        }
        break;
    case BSP_STEP_02:
        if (value == 0)
        {
            randomize_503_seq(data);
        }
        break;
    case BSP_STEP_03:
        randomize_522_seq(data);
        break;
    case BSP_STEP_13:
        if (value == 0)
        {
            root_rocket_seq(&data);
        }
        break;
    case BSP_STEP_14:
        if (value == 0)
        {
            modify_rocket_seq(data);
        }
        break;
    case BSP_STEP_15:
        if (value == 0)
        {
            modify_rocket_seq(data);
        }
        break;
    case BSP_STEP_16:
        if (value == 0)
        {
            randomize_rocket_seq(&data);
        }
        break;
    default:
        break;
    }
}

void handleStop()
{
    all_notes_off(data.storage_503, MIDI_CHANNEL_503);
    all_notes_off(data.storage_522, MIDI_CHANNEL_522);
    all_notes_off(data.storage_p50, MIDI_CHANNEL_P50);
    all_notes_off(data.storage_rocket, MIDI_CHANNEL_ROCKET);
    data.step = 0;
    data.ticks_counter = 0;
}

void setup() {
    MIDI.setHandleNoteOn(handleNoteOn);
    MIDI.setHandleNoteOff(handleNoteOff);
    MIDI.setHandleClock(handleClock);
    MIDI.setHandleControlChange(handleControlChange);
    MIDI.setHandleStop(handleStop);

    MIDI.begin(MIDI_CHANNEL_OMNI);

    data.rocket_pattern = init_bassline();
  
    data.step = 0;
    data.ticks_counter = 0;
    data.swing = 0;

    data.rocket_density = .8;
    data.rocket_octave = 3;
    data.p50_octave = 5;
    data.root = ROOT_C;
    data.scale = get_scale(AEOLIAN);

    init_storage(data.storage_503);
    init_storage(data.storage_522);
    init_storage(data.storage_p50);
    init_storage(data.storage_rocket);

    data.uiState = init_ui_state();

    // Initialize patterns
    randomize_503_seq(data);
    randomize_522_seq(data);
    randomize_P50_seq(&data);
    randomize_rocket_seq(&data);
    root_rocket_seq(&data);

}

void note_on(uint8_t note, uint8_t velocity, uint8_t channel, uint8_t* storage)
{
    uint8_t stored = pop_from_storage(storage, note);
    if (stored > 0)
    {
        MIDI.sendNoteOff(note, 0, channel);
    }
    MIDI.sendNoteOn(note, velocity, channel);
    add_to_storage(storage, note);
}

void note_off(uint8_t note, uint8_t channel, uint8_t* storage)
{
    MIDI.sendNoteOff(note, 0, channel);
    uint8_t stored = pop_from_storage(storage, note);
}

void send_cc(uint8_t cc, uint8_t value, uint8_t channel)
{
    MIDI.sendControlChange(cc, value, channel);
}

void all_notes_off(uint8_t* storage, uint8_t channel)
{
    uint8_t p = 0;
    do {
        p = pop_from_storage(storage);
        if (p > 0)
        {
            MIDI.sendNoteOff(p, 0, channel);
        }
    } while (p != 0);
}

void play_all()
{
    play_503(data);
    play_522(data);
    play_rocket(&data);
    play_P50(&data);
}

void loop() {
    MIDI.read();
}
