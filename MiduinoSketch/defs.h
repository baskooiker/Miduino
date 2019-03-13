#pragma once

#include "consts.h"
#include "enums.h"
#include "note_struct.h"
#include "midi_channel.h"
#include "rand.h"

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

typedef uint16_t BinaryPattern;

class GatePattern16
{
public:
    BinaryPattern pattern;
    uint8_t length;

    GatePattern16()
    {
        pattern = 0x00;
        length = 16;
    }

    ~GatePattern16() {}
};

 class GatePatternAB 
 {
 public:
    BinaryPattern patterns[3];
    uint8_t abPattern[4];
    TimeDivision time_division;
    uint8_t length;

    GatePatternAB()
    {
        patterns[0] = 0x00;
        patterns[1] = 0x00;
        patterns[2] = 0x00;
        length = 64;
        time_division = TimeDivision::Sixteenth;
    }
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

class BassDubSettings {
public:
    BassDubStyle style;
    NoteInterval note_interval;
    GatePatternAB hit_probs;
    uint8_t density;
    uint8_t v_pitch;
    uint8_t fugue_id;
    PitchStorage storage;

    BassDubSettings()
    {
        style = BassDubStyle::DubOctave;
        note_interval = NoteInterval::IntervalRoot;
        density = 0;
        v_pitch = 0;
    }
};

class Mfb522Settings 
{
public:
    GatePattern16 ac_522_pattern;
    GatePatternAB bd_522_pattern;
    GatePattern16 lo_tom_522_pattern;
    GatePattern16 mi_tom_522_pattern;
    GatePattern16 rs_522_pattern;
    GatePatternAB clave_522_pattern;
    GatePatternAB clap_522_pattern;
    GatePatternAB hh_522_pattern;
    GatePatternAB oh_522_pattern;
    GatePatternAB cy_522_pattern;
    GatePatternAB sd_522_pattern;

    bool use_hh_int;
    IntervalPattern hh_int_pattern;

    PitchStorage storage;

    Mfb522Settings()
    {
        use_hh_int = false;
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

class PolySettings 
{
public:
    GatePatternAB gates_low;
    GatePatternAB gates;
    GatePatternAB tie_pattern;
    uint8_t pitch_offset;
    PolyType type;
    PitchStorage storage;

    PolySettings()
    {
        pitch_offset = 48;
        type = PolyType::PolyLow;
    }
};

class MicroTimingStruct 
{
public:
    int8_t shuffle_off;
    uint8_t delay;
};

class TanzbarTimeSettings 
{
public:
    MicroTimingStruct bd;
    MicroTimingStruct sd;
    MicroTimingStruct cp;
    MicroTimingStruct hh;
    MicroTimingStruct tc;
    MicroTimingStruct ma;
    MicroTimingStruct cl;
    MicroTimingStruct cb;
    MicroTimingStruct cy;
};

class MonoSettings 
{
public:
    MonoStyle style;
    ArpData arp_data;
    IntervalPattern int_pattern;
    GatePatternAB euclid_pattern;
    IntervalPattern lead_pattern;

    uint8_t variable_pitch_offset;
    uint8_t variable_density;

    uint8_t pitch_offset;
    uint8_t fugue_id;

    PitchStorage storage;

    MonoSettings()
    {
        style = MonoStyle::MonoSixteenths;
        fugue_id = 0;
    }
};

class MonoDubSettings 
{
public:
    uint8_t variable_pitch_offset;
    MonoSettings settings;
    MonoDubStyle style;

    MonoDubSettings()
    {
        variable_pitch_offset = 0;
        style = MonoDubStyle::MonoDubLead;
    }
};
