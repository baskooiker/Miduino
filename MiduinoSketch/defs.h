#pragma once

#include "consts.h"
#include "enums.h"

class Coefficients {
public:
    float one;
    float two;
    float three;
    float four;
    float eights;
    float up;
    float down;
};

class RandomParam {
public:
  uint8_t note;
  uint8_t min;
  uint8_t max;  
};

class IntervalPattern {
public:
    TimeDivision pattern[16];
    TimeDivision time_division;
    uint8_t length;

    IntervalPattern()
    {
        length = 16;
        for (int i = 0; i < length; i++)
        {
            pattern[i] = TimeDivision::Sixteenth;
        }
        time_division = TimeDivision::Eight;
    }
};

class IntervalProbs 
{
public:
    uint8_t p_4;
    uint8_t p_8;
    uint8_t p_16;
    uint8_t p_32;
    uint8_t p_t8;
};

static const IntervalProbs arp_interval_probs = { 25, 25, 25, 0,  0 };

class ButtonState
{
public:
    unsigned long last_pressed;
    unsigned long last_released;
};

class UiState {
public:
    uint16_t bsp_button_state;

    ButtonState pad_state[NR_OF_PADS];
    ButtonState step_state[NR_OF_STEPS];
};

class FuguePlayerSettings {
public:
    uint8_t pitch_offset;
    uint8_t manual_pitch_offset;
    uint8_t length;
    FuguePlayerType type;
    uint32_t counter;
    uint8_t rhythm;
    uint8_t density;
    NoteInterval note_interval;
    uint8_t note_repeat;

    FuguePlayerSettings()
    {
        pitch_offset = 36;
        length = 4;
        type = FuguePlayerType::FugueForward;
        note_interval = NoteInterval::IntervalRoot;
        note_repeat = 1;
    }
};

class ArpData
{
public:
    uint8_t min;
    uint8_t range;
    uint8_t range_count;
    uint8_t counter;
    ArpType type;
    RangeType range_type;
    uint8_t last_note;

    uint8_t arp_notes[32];
    uint8_t arp_notes_length;

    ArpData()
    {
        min = 36;
        range = 12;
        range_count = 3;
        counter = 0;
        type = ArpType::UP;
        range_type = RangeType::Range;
        last_note = 0;

        arp_notes_length = 0;
    }
};
