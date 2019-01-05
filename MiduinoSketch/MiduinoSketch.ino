#include <stdlib.h>

#include <MIDI.h>

#include "basslines.h"
#include "defs.h"
#include "init.h"
#include "rhythms.h"
#include "storage.h"
#include "scales.h"
#include "ui.h"

#include "lead.h"
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
        data.uiState.pad_state[0].last_pressed = millis();
        if (data.uiState.control_mode == CONTROL_MODE_ROOT)
        {
            data.scale.root = ROOT_C;
        }
        if (data.uiState.control_mode == CONTROL_MODE_NORMAL)
        {
            data.uiState.kill_low = !data.uiState.kill_low;
        }
        set_pad_state(data.uiState, 0, true);
        break;
    case BSP_PAD_02:
        data.uiState.pad_state[1].last_pressed = millis();
        if (data.uiState.control_mode == CONTROL_MODE_ROOT)
        {
            data.scale.root = ROOT_D;
        }
        if (data.uiState.control_mode == CONTROL_MODE_NORMAL)
        {
            data.uiState.kill_mid = !data.uiState.kill_mid;
        }
        set_pad_state(data.uiState, 1, true);
        break;
    case BSP_PAD_03:
        data.uiState.pad_state[2].last_pressed = millis();
        if (data.uiState.control_mode == CONTROL_MODE_ROOT)
        {
            data.scale.root = ROOT_E;
        }
        if (data.uiState.control_mode == CONTROL_MODE_NORMAL)
        {
            data.uiState.kill_perc = !data.uiState.kill_perc;
        }
        set_pad_state(data.uiState, 2, true);
        break;
    case BSP_PAD_04:
        data.uiState.pad_state[3].last_pressed = millis();
        if (data.uiState.control_mode == CONTROL_MODE_ROOT)
        {
            data.scale.root = ROOT_F;
        }
        if (data.uiState.control_mode == CONTROL_MODE_NORMAL)
        {
            data.uiState.kill_high = !data.uiState.kill_high;
        }
        set_pad_state(data.uiState, 3, true);
        break;
    case BSP_PAD_05:
        data.uiState.pad_state[4].last_pressed = millis();
        if (data.uiState.control_mode == CONTROL_MODE_ROOT)
        {
            data.scale.root = ROOT_G;
        }
        set_pad_state(data.uiState, 4, true);
        break;
    case BSP_PAD_06:
        data.uiState.pad_state[5].last_pressed = millis();
        if (data.uiState.control_mode == CONTROL_MODE_ROOT)
        {
            data.scale.root = ROOT_A;
        }
        set_pad_state(data.uiState, 5, true);
        break;
    case BSP_PAD_07:
        data.uiState.pad_state[6].last_pressed = millis();
        if (data.uiState.control_mode == CONTROL_MODE_ROOT)
        {
            data.scale.root = ROOT_B;
        }
        set_pad_state(data.uiState, 6, true);
        break;
    case BSP_PAD_08:
        data.uiState.pad_state[7].last_pressed = millis();
        set_pad_state(data.uiState, 7, true);
        break;
    case BSP_PAD_09:
        data.uiState.pad_state[8].last_pressed = millis();
        if (data.uiState.control_mode == CONTROL_MODE_ROOT)
        {
            data.scale.root = ROOT_C_SHARP;
        }
        else
        {
            data.uiState.drum_fill = true;
        }
        set_pad_state(data.uiState, 8, true);
        break;
    case BSP_PAD_10:
        data.uiState.pad_state[9].last_pressed = millis();
        if (data.uiState.control_mode == CONTROL_MODE_ROOT)
        {
            data.scale.root = ROOT_D_SHARP;
        }
        set_pad_state(data.uiState, 9, true);
        break;
    case BSP_PAD_11:
        data.uiState.pad_state[10].last_pressed = millis();
        set_pad_state(data.uiState, 10, true);
        break;
    case BSP_PAD_12:
        data.uiState.pad_state[11].last_pressed = millis();
        if (data.uiState.control_mode == CONTROL_MODE_ROOT)
        {
            data.scale.root = ROOT_F_SHARP;
        }
        set_pad_state(data.uiState, 11, true);
        break;
    case BSP_PAD_13:
        data.uiState.pad_state[12].last_pressed = millis();
        if (data.uiState.control_mode == CONTROL_MODE_ROOT)
        {
            data.scale.root = ROOT_G_SHARP;
        }
        set_pad_state(data.uiState, 12, true);
        break;
    case BSP_PAD_14:
        data.uiState.pad_state[13].last_pressed = millis();
        if (data.uiState.control_mode == CONTROL_MODE_ROOT)
        {
            data.scale.root = ROOT_A_SHARP;
        }
        set_pad_state(data.uiState, 13, true);
        break;
    case BSP_PAD_15:
        data.uiState.pad_state[14].last_pressed = millis();
        set_pad_state(data.uiState, 14, true);
        break;
    case BSP_PAD_16:
        data.uiState.pad_state[15].last_pressed = millis();
        if (data.uiState.control_mode == CONTROL_MODE_NORMAL)
        {
            data.uiState.control_mode = CONTROL_MODE_ROOT;
        }
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

void handleNoteOff(byte channel, byte pitch, byte velocity)
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
        boolean p_long = was_pressed_long(data.uiState.pad_state[7]);
        if (data.uiState.control_mode == CONTROL_MODE_ROOT)
        {
            data.uiState.control_mode = CONTROL_MODE_NORMAL;
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
        boolean p_long = was_pressed_long(data.uiState.pad_state[9]);
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

void stop_notes_all_instruments()
{
    stop_notes(data.settings_503.storage, MIDI_CHANNEL_503);
    stop_notes(data.settings_522.storage, MIDI_CHANNEL_522);
    stop_notes(data.settings_p50.storage, MIDI_CHANNEL_P50);
    stop_notes(data.settings_rocket.storage, MIDI_CHANNEL_ROCKET);
    stop_notes(data.settings_lead.storage, MIDI_CHANNEL_LEAD);
}

void handleClock()
{
    stop_notes_all_instruments();
    play_all();

    data.ticks += 1;
    if (data.ticks >= TICKS_PER_STEP)
    {
        data.ticks = 0;
        data.step = (data.step + 1) % COMMON_DENOMINATOR;
    }
}

void handleControlChange(byte channel, byte number, byte value)
{
    switch (number)
    {
    case BSP_KNOB_01:
        send_cc(BD_DECAY, value, MIDI_CHANNEL_503);
        break;
    case BSP_KNOB_05:
        data.settings_p50.play_chords = value > 0;
        data.settings_p50.chords_velocity = value;
        break;  
    case BSP_KNOB_06:
    {
        if (value < 64)
            data.settings_lead.style = LeadStyle::LeadIntPattern;
        else
            data.settings_lead.style = LeadStyle::LeadSixteenths;
        break;
    }
    case BSP_KNOB_07:
        data.settings_rocket.pitch_range = value;
        break;
    case BSP_KNOB_08:
        data.settings_rocket.gate_density = value / 2 + 64;
        if (value < 10)
            data.settings_rocket.style = RocketWhole;
        else if (value < 64)
            data.settings_rocket.style = RocketArpInterval;
        else if (value < 120)
            data.settings_rocket.style = RocketProb;
        else
            data.settings_rocket.style = RocketSixteenths;
        break;
    case BSP_KNOB_14:
        data.settings_lead.arp_data.range = 12 + (uint8_t)(value * 24. / 127.);
        break;
    case BSP_KNOB_16:
        if (value < 10)
        {
            data.settings_rocket.note_range = NoteRange::RangeRoot;
        }
        else if (value < 120)
        {
            data.settings_rocket.note_range = NoteRange::RangeChord;
        }
        else
        {
            data.settings_rocket.note_range = NoteRange::RangeScale;
        }
        break;
    case BSP_STEP_01:
        if (value == 0)
        {
            randomize_503_sound();
            randomize_503_seq(data);
            randomize_522_seq(data);
        }
        else
        {
            data.uiState.step_state[0].last_pressed = millis();
        }
        break;
    case BSP_STEP_02:
        if (value == 0)
        {
        }
        else
        {
            data.uiState.step_state[1].last_pressed = millis();
        }
        break;
    case BSP_STEP_03:
        if (value == 0)
        {
        }
        else
        {
            data.uiState.step_state[2].last_pressed = millis();
        }
        break;
    case BSP_STEP_09:
        if (value == 0)
        {
            set_chords(data.harmony, 0);
            data.harmony.pitches.time_division = TimeDivision::TIME_DIVISION_SIXTEENTH;

            randomize_P50_seq(data);
        }
        else
        {
            data.uiState.step_state[8].last_pressed = millis();
        }
        break;
    case BSP_STEP_10:
        if (value == 0)
        {
            set_chord_pattern_ab(data.harmony);
            if (randf() < .5)
            {
                set_ab_pattern_low(data.harmony.pitches.abPattern);
                data.harmony.pitches.time_division = TimeDivision::TIME_DIVISION_SIXTEENTH;
            }
            else
            {
                set_ab_pattern_high(data.harmony.pitches.abPattern);
                data.harmony.pitches.time_division = TimeDivision::TIME_DIVISION_EIGHT;
            }
        }
        else
        {
            data.uiState.step_state[9].last_pressed = millis();
        }
        break;
    case BSP_STEP_11:
        if (value == 0)
        {
            set_chord_pattern_ab(data.harmony);
            set_ab_pattern_high(data.harmony.pitches.abPattern);
            data.harmony.pitches.time_division = TimeDivision::TIME_DIVISION_SIXTEENTH;
        }
        else
        {
            data.uiState.step_state[10].last_pressed = millis();
        }
        break;
    case BSP_STEP_12:
        if (value == 0)
        {
            randomize_lead(data);
        }
        break;
    case BSP_STEP_13:
        if (value == 0)
        {
            randomize_rocket_seq(data);
        }
        else
        {
            data.uiState.step_state[12].last_pressed = millis();
        }
        break;
    case BSP_STEP_14:
        if (value == 0)
        {
            modify_rocket_seq(data);
        }
        else
        {
            data.uiState.step_state[13].last_pressed = millis();
        }
        break;
    case BSP_STEP_15:
    case BSP_STEP_16:
        if (value == 0)
        {
            randomize_rocket_seq(data);
        }
        break;
    default:
        break;
    }
}

void handleStop()
{
    all_notes_off(data.settings_503.storage, MIDI_CHANNEL_503);
    all_notes_off(data.settings_522.storage, MIDI_CHANNEL_522);
    all_notes_off(data.settings_p50.storage, MIDI_CHANNEL_P50);
    all_notes_off(data.settings_rocket.storage, MIDI_CHANNEL_ROCKET);
    all_notes_off(data.settings_lead.storage, MIDI_CHANNEL_LEAD);

    data.step = 0;
    data.ticks = 0;
}

void setup() {
    MIDI.setHandleNoteOn(handleNoteOn);
    MIDI.setHandleNoteOff(handleNoteOff);
    MIDI.setHandleClock(handleClock);
    MIDI.setHandleControlChange(handleControlChange);
    MIDI.setHandleStop(handleStop);

    MIDI.begin(MIDI_CHANNEL_OMNI);

    srand(analogRead(A0));

    data = init_application_data();

    // Initialize patterns
    send_cc(BD_DECAY, randi(32, 64), MIDI_CHANNEL_503);
    randomize_503_seq(data);
    randomize_522_seq(data);
    randomize_P50_seq(data);
    randomize_rocket_seq(data);
    randomize_lead(data);
}

void note_on(uint8_t note, uint8_t velocity, uint8_t channel, PitchStorage& storage, uint8_t length)
{
    NoteStruct stored = pop_from_storage(storage, note);
    if (stored.pitch > 0)
    {
        MIDI.sendNoteOff(note, 0, channel);
    }
    MIDI.sendNoteOn(note, velocity, channel);
    add_to_storage(storage, note, length);
}

void note_off(uint8_t note, uint8_t channel, PitchStorage& storage)
{
    MIDI.sendNoteOff(note, 0, channel);
    NoteStruct stored = pop_from_storage(storage, note);
}

void send_cc(uint8_t cc, uint8_t value, uint8_t channel)
{
    MIDI.sendControlChange(cc, value, channel);
}

void all_notes_off(PitchStorage& storage, uint8_t channel)
{
    NoteStruct p = {0, 0};
    do {
        p = pop_from_storage(storage);
        if (p.pitch > 0)
        {
            MIDI.sendNoteOff(p.pitch, 0, channel);
        }
    } while (p.pitch != 0);
}

void play_all()
{
    play_503(data);
    play_522(data);
    play_rocket(data);
    play_P50(data);
    play_lead(data);
}

void loop() {
    MIDI.read();
}
