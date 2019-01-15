#pragma once

#include "consts.h"
#include "enums.h"

typedef struct {
    float one;
    float two;
    float three;
    float four;
    float eights;
    float up;
    float down;
} Coefficients;

typedef struct {
  uint8_t note;
  uint8_t min;
  uint8_t max;  
} RandomParam;

typedef struct {
    uint8_t data[16];
    uint8_t length;
} RhythmPattern;

#define NOTES_IN_BAR (16)
typedef uint8_t CvPattern[NOTES_IN_BAR];

typedef struct {
    uint8_t pattern[16];
    uint8_t length;
} CvPattern16;

typedef struct {
    uint8_t pattern[64];
    uint8_t length;
} CvPattern64;

typedef struct {
    CvPattern patterns[3];
    uint8_t abPattern[4];
    TimeDivision time_division;
    uint8_t length;
} CvPatternAB;

typedef uint16_t BinaryPattern;

typedef struct {
    BinaryPattern pattern;
    uint8_t length;
} GatePattern16;

typedef struct {
    BinaryPattern patterns[4];
    uint8_t length;
} GatePattern64;

typedef struct {
    BinaryPattern patterns[3];
    uint8_t abPattern[4];
    TimeDivision time_division;
    uint8_t length;
} GatePatternAB;

#define HOLD_NOTE 0xFF
#define TIE_NOTE 0xFE

typedef struct {
    uint8_t pitch;
    uint8_t velocity;
    uint8_t length;
} NoteStruct;

#define STORAGE_SIZE 16

typedef struct {
    NoteStruct data[STORAGE_SIZE];
    uint8_t size;
} PitchStorage;

typedef struct {
    TimeDivision pattern[16];
    TimeDivision time_division;
    uint8_t length;
} IntervalPattern;

typedef struct {
    uint8_t p_4;
    uint8_t p_8;
    uint8_t p_16;
    uint8_t p_32;
    uint8_t p_t8;
} IntervalProbs;
static const IntervalProbs hat_interval_probs = { 0,  20, 50, 15, 10 };
static const IntervalProbs arp_interval_probs = { 25, 25, 25, 0,  0 };

typedef struct {
    unsigned long last_pressed;
} ButtonState;

typedef struct {
    uint16_t bsp_button_state;
    uint16_t bsp_pad_state;

    ButtonState step_state[16];
    ButtonState pad_state[16];

    ControlMode control_mode;

    bool kill_low;
    bool kill_mid;
    bool kill_high;
    bool kill_perc;

    bool drum_fill;
    bool drum_roll;
    uint8_t bd_decay_factor;
} UiState;

typedef struct {
    GatePatternAB accents;
    CvPatternAB pitches;
    CvPatternAB octaves;
    CvPatternAB variable_octaves;
    GatePatternAB slides;

    CvPatternAB probs;
    IntervalPattern int_pattern;
    GatePatternAB euclid_pattern;

    uint8_t gate_density;
    uint8_t low_velocity;
    uint8_t high_velocity;
    uint8_t pitch_range;
    RocketStyle style;
    CvPatternAB note_range_prob;
    uint8_t note_range_value;

    PitchStorage storage;
} SettingsRocket;

typedef struct {
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
} Settings522;

typedef struct {
    uint8_t min;
    uint8_t range;
    uint8_t counter;
    ArpType type;
    uint8_t last_note;
} ArpData;

typedef struct {
    GatePatternAB gates_low;
    GatePatternAB gates;
    uint8_t octave;
    PolyType type;
    PitchStorage storage;
} SettingsP50;

typedef struct {
    uint8_t notes[8];
    uint8_t length;
    Root root;
} Scale;

typedef struct {
    GatePattern16 ac_pattern;
    GatePatternAB bd_pattern;
    GatePatternAB sd_pattern;
    GatePatternAB hh_pattern;
    GatePatternAB oh_pattern;

    uint8_t bd_decay;

    PitchStorage storage;
} Settings503;

typedef struct {
    ArpData arp_data;
    IntervalPattern int_pattern;
    IntervalPattern long_pattern;
    CvPatternAB min_pitch_pattern;
    LeadStyle style;
    PitchStorage storage;
} SettingsLead;

typedef struct {
    ArpData arp_data;
    IntervalPattern int_pattern;
    PitchStorage storage;
} SettingsMono;

typedef struct {
    Scale scale;
    uint8_t ticks;
    long step;

    CvPatternAB harmony;

    SettingsP50 settings_p50;
    Settings522 settings_522;
    Settings503 settings_503;
    SettingsRocket settings_rocket;
    SettingsLead settings_lead;
    SettingsMono settings_mono;

    UiState uiState;
} ApplicationData;
