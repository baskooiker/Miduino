#pragma once

#include "consts.h"
#include "enums.h"

typedef struct {
    uint32_t step;
    uint8_t tick;
    PlayState state;
} TimeStruct;

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

#define STEPS_IN_BAR (16)
#define TICKS_IN_BAR (96)
typedef uint8_t CvPattern[STEPS_IN_BAR];

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

typedef struct {
    uint8_t pitch;
    uint8_t velocity;
    uint8_t length;
    NoteType type;
} NoteStruct;

#define STORAGE_SIZE 16

typedef struct {
    NoteStruct data[STORAGE_SIZE];
    uint8_t size;
    uint8_t channel;
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
static const IntervalProbs arp_interval_probs = { 25, 25, 25, 0,  0 };

typedef struct {
    unsigned long last_pressed;
} ButtonState;

typedef struct {
    uint16_t bsp_button_state;
    uint16_t bsp_pad_state;

    ButtonState pad_state[16];

    bool kill_low;
    bool kill_mid;
    bool kill_high;
    bool kill_perc;

    //bool kill_bass;

    bool drum_fill;
    uint8_t drum_roll;
    uint8_t bd_decay_factor;
    uint8_t poly_pitch_offset;
} UiState;

typedef struct {
    uint8_t pitch_offset;
    uint8_t length;
    FuguePlayerType type;
    uint32_t counter;
    uint8_t rhythm;
    //uint8_t note_repeat;
    NoteInterval interval;
} FuguePlayerSettings;

#define NUMBER_FUGUE_PLAYERS 4

typedef struct {
    CvPattern16 pattern;
    FuguePlayerSettings player_settings[NUMBER_FUGUE_PLAYERS];
} FugueSettings;

typedef struct {
    GatePatternAB accents;
    CvPatternAB pitches;
    CvPatternAB octaves;
    CvPatternAB variable_octaves;
    GatePatternAB slides;

    CvPatternAB probs;
    IntervalPattern int_pattern;
    GatePatternAB euclid_pattern;
    GatePatternAB low_pattern;

    uint8_t pitch_range;
    BassStyle style;
    CvPatternAB note_range_prob;
    uint8_t note_range_value;
    uint8_t octave_offset;
    uint8_t fugue_id;

    bool kill;

    PitchStorage storage;
} BassSettings;

typedef struct {
    BassDubStyle style;
    NoteInterval note_interval;
    GatePatternAB octave_probs;
    GatePatternAB hit_probs;
    uint8_t fugue_id;
    PitchStorage storage;
} BassDubSettings;

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
} Mfb522Settings;

typedef struct {
    uint8_t min;
    uint8_t range;
    uint8_t range_count;
    uint8_t counter;
    ArpType type;
    RangeType range_type;
    uint8_t last_note;

    uint8_t arp_notes[32];
    uint8_t arp_notes_length;
} ArpData;

typedef struct {
    GatePatternAB gates_low;
    GatePatternAB gates;
    GatePatternAB tie_pattern;
    uint8_t pitch_offset;
    PolyType type;
    PitchStorage storage;
} PolySettings;

typedef struct {
    uint8_t notes[8];
    uint8_t length;
    Root root;
} Scale;

typedef struct {
    CvPatternAB const_pattern;
    CvPatternAB low_pattern;
    CvPatternAB high_pattern;
    HarmonyType type;
    Scale scale;
} HarmonyStruct;

typedef struct {
    GatePattern16 ac_pattern;
    GatePatternAB bd_pattern;
    GatePatternAB sd_pattern;
    GatePatternAB hh_pattern;
    GatePatternAB oh_pattern;
    GatePatternAB cy_pattern;
    CvPatternAB tom_pattern;

    IntervalPattern hat_int_pattern;

    uint8_t bd_decay;
    bool play_pitch_bd;

    uint8_t volume_cy;
    HatStyle hat_style;

    uint8_t volume_tom;
    uint8_t nr_toms;
    uint8_t toms_offset;
    GatePatternAB tom_mask;

    PitchStorage storage;
} Mfb503Settings;

typedef struct {
    ArpData arp_data;
    CvPatternAB min_pitch_pattern;
    GatePatternAB pattern_slow;
    LeadStyle style;
    PitchStorage storage;
} LeadSettings;

typedef struct {
    MonoStyle style;
    ArpData arp_data;
    IntervalPattern int_pattern;
    GatePatternAB euclid_pattern;
    IntervalPattern lead_pattern;

    uint8_t variable_pitch_offset;
    uint8_t pitch_offset;
    uint8_t fugue_id;

    PitchStorage storage;
} MonoSettings;

typedef struct {
    MonoSettings settings;
    MonoDubStyle style;
} MonoDubSettings;

typedef struct {
    TimeStruct time;
    HarmonyStruct harmony;

    FugueSettings fugue_settings;

    PolySettings poly_settings;
    Mfb522Settings mfb_522_settings;
    Mfb503Settings mfb_503_settings;
    BassSettings bass_settings;
    BassDubSettings bass_dub_settings;
    LeadSettings lead_settings;
    MonoSettings mono_settings;
    MonoDubSettings mono_dub_settings;

    UiState ui_state;
} ApplicationData;
