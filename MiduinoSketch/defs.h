#pragma once

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define CLIP(value,minimum,maximum) (MIN(MAX(value, minimum), maximum))

#define MIDI_CHANNEL_ROCKET 1
#define MIDI_CHANNEL_P50    6
#define MIDI_CHANNEL_LEAD   7
#define MIDI_CHANNEL_503    10
#define MIDI_CHANNEL_522    12

#define COMMON_DENOMINATOR 240240 // Exept 9
#define TICKS_PER_STEP 6

// MIDI 503
#define NOTE_503_BD 36
#define NOTE_503_SD 38
#define NOTE_503_LT 41
#define NOTE_503_MT 43
#define NOTE_503_HT 45 
#define NOTE_503_CY 46
#define NOTE_503_OH 39
#define NOTE_503_HH 40

// MIDI 522
#define NOTE_522_BD_SHORT 35
#define NOTE_522_BD_LONG  36
#define NOTE_522_RS       37
#define NOTE_522_SN       38
#define NOTE_522_CP_SHORT 39
#define NOTE_522_CP_LONG  40
#define NOTE_522_CB       41
#define NOTE_522_HH       42
#define NOTE_522_OH       46
#define NOTE_522_CLAVE    43
#define NOTE_522_LO_TOM   45
#define NOTE_522_MI_TOM   47
#define NOTE_522_HI_TOM   48
#define NOTE_522_HI2_TOM  50
#define NOTE_522_CYMBAL   49

// BSP CC's
#define BSP_STEP_01 20
#define BSP_STEP_02 21
#define BSP_STEP_03 22
#define BSP_STEP_04 23
#define BSP_STEP_05 24
#define BSP_STEP_06 25
#define BSP_STEP_07 26
#define BSP_STEP_08 27
#define BSP_STEP_09 28
#define BSP_STEP_10 29
#define BSP_STEP_11 30
#define BSP_STEP_12 31
#define BSP_STEP_13 52
#define BSP_STEP_14 53
#define BSP_STEP_15 54
#define BSP_STEP_16 55

#define BSP_KNOB_01 10
#define BSP_KNOB_02 74
#define BSP_KNOB_03 71
#define BSP_KNOB_04 76
#define BSP_KNOB_05 77
#define BSP_KNOB_06 93
#define BSP_KNOB_07 73
#define BSP_KNOB_08 75
#define BSP_KNOB_09 114
#define BSP_KNOB_10 18
#define BSP_KNOB_11 19
#define BSP_KNOB_12 16
#define BSP_KNOB_13 17
#define BSP_KNOB_14 91
#define BSP_KNOB_15 79
#define BSP_KNOB_16 72

#define BSP_PAD_01 36
#define BSP_PAD_02 37
#define BSP_PAD_03 38
#define BSP_PAD_04 39
#define BSP_PAD_05 40
#define BSP_PAD_06 41
#define BSP_PAD_07 42
#define BSP_PAD_08 43
#define BSP_PAD_09 44
#define BSP_PAD_10 45
#define BSP_PAD_11 46
#define BSP_PAD_12 47
#define BSP_PAD_13 48
#define BSP_PAD_14 49
#define BSP_PAD_15 50
#define BSP_PAD_16 51

// MFB 503 CC's
#define BD_LEVEL  8 
#define BD_TUNE   9 
#define BD_DECAY  10 
#define BD_PITCH  11 
#define BD_DRIVE  12 
#define BD_ATTACK 13 

#define SD_LEVEL  16
#define SD_TUNE   17
#define SD_DECAY  18
#define SD_NOISE  19 

#define HH_LEVEL  56
#define HH_MIX    57
#define OH_DECAY  58
#define HH_DECAY  59

///////////
// Enums
///////////

enum ControlMode
{
    CONTROL_MODE_NORMAL,
    CONTROL_MODE_ROOT
};

enum Root {
    ROOT_C = 0,
    ROOT_C_SHARP,
    ROOT_D,
    ROOT_D_SHARP,
    ROOT_E,
    ROOT_F,
    ROOT_F_SHARP,
    ROOT_G,
    ROOT_G_SHARP,
    ROOT_A,
    ROOT_A_SHARP,
    ROOT_B
};

enum TimeDivision {
    TIME_DIVISION_THIRTYTWO = 32,
    TIME_DIVISION_SIXTEENTH = 16,
    TIME_DIVISION_EIGHT = 8,
    TIME_DIVISION_FOURTH = 4,
    TIME_DIVISION_HALF = 2,
    TIME_DIVISION_WHOLE = 1,
    TIME_DIVISION_TRIPLE_EIGHT = 12
};

////////////
// Structs
////////////

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

typedef struct {
    uint8_t pitch;
    uint8_t length;
} NoteStruct;

#define STORAGE_SIZE 16

typedef struct {
    NoteStruct data[STORAGE_SIZE];
    uint8_t size;
} PitchStorage;

typedef struct {
    CvPattern64 pitches;
    GatePattern64 gates;
} ChordPattern;

typedef struct {
    CvPatternAB pitches;
} ChordPatternAB;

typedef struct {
    TimeDivision pattern[16];
    TimeDivision time_division;
} IntervalPattern;

typedef struct {
    float p_t8;
    float p_4;
    float p_8;
    float p_32;
} IntervalProbs;
static const IntervalProbs hat_interval_probs = { .05f, .0f , .2f, .15f };
static const IntervalProbs arp_interval_probs = { .0f , .15f, .3f, .0f  };

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
} UiState;

typedef struct {
    GatePattern64 accents;
    CvPatternAB pitches;
    CvPatternAB octaves;
    CvPatternAB variable_octaves;
    GatePatternAB slides;
    CvPatternAB probs;

    IntervalPattern int_pattern;

    uint8_t gate_density;
    uint8_t low_velocity;
    uint8_t high_velocity;
    uint8_t pitch_range;
    bool use_int_pattern;

    PitchStorage storage;
} SettingsRocket;

typedef struct {
    GatePattern16 ac_522_pattern;
    GatePatternAB bd_522_pattern;
    GatePattern16 lo_tom_522_pattern;
    GatePattern16 mi_tom_522_pattern;
    GatePattern16 rs_522_pattern;
    GatePattern64 clave_522_pattern;
    GatePattern16 clap_522_pattern;
    GatePattern64 hh_522_pattern;
    GatePattern64 oh_522_pattern;
    GatePatternAB cy_522_pattern;
    GatePattern64 sd_522_pattern;

    bool use_hh_int;
    IntervalPattern hh_int_pattern;


    PitchStorage storage;
} Settings522;

enum ArpType {
    UP,
    DOWN,
    UPDOWN,
    PICKING_IN,
    PICKING_OUT,
    CLOSEST,
    RANDOM
};

typedef struct {
    uint8_t min;
    uint8_t range;
    uint8_t counter;
    ArpType type;
    uint8_t last_note;
} ArpData;

typedef struct {
    GatePatternAB gates;
    uint8_t octave;
    bool play_chords;
    bool play_arp;
    uint8_t arp_velocity;
    uint8_t chords_velocity;
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
    GatePattern16 sd_pattern;
    GatePatternAB hh_pattern;
    GatePatternAB oh_pattern;

    PitchStorage storage;
} Settings503;

typedef struct {
    ArpData arp_data;

    PitchStorage storage;
} SettingsLead;

typedef struct {
    Scale scale;
    uint8_t ticks;
    long step;

    ChordPatternAB harmony;

    SettingsP50 settings_p50;
    Settings522 settings_522;
    Settings503 settings_503;
    SettingsRocket settings_rocket;
    SettingsLead settings_lead;

    UiState uiState;
} ApplicationData;
