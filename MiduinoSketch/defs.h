#ifndef DEFS_H
#define DEFS_H

#include "scales.h"

#define MIDI_CHANNEL_ROCKET 1
#define MIDI_CHANNEL_P50    5
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

#define CC_CUTOFF    74
#define CC_RESONANCE 71
#define CC_ENV_MOD   73
#define CC_OSC_WAVE  70
#define CC_OSC_TUNE  79

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

typedef struct {
  uint8_t note;
  uint8_t min;
  uint8_t max;  
} RandomParam;

typedef struct {
    uint8_t data[16];
    uint8_t length;
} RhythmPattern;

typedef struct {
    uint8_t pattern[16];
    uint8_t length;
} CvPattern16;

typedef struct {
    uint8_t pattern[64]; 
    uint8_t length;
} CvPattern64;

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
} GatePatternAB;

#define STORAGE_SIZE 16
typedef uint8_t PitchStorage[STORAGE_SIZE];

typedef struct {
    GatePattern64 accents;
    CvPattern16 pitches;
    GatePattern64 gates;
    GatePattern64 slides;
} Bassline;

typedef struct {
    CvPattern64 pitches;
    GatePattern64 gates;
} ChordPattern;

typedef struct {
    uint16_t bsp_button_state;
    uint16_t bsp_pad_state;

    boolean in_root_mode;
    boolean in_swing_mode;
} UiState;

UiState init_ui_state()
{
    UiState ui_state = {0};
    ui_state.bsp_button_state = 0x00;
    ui_state.bsp_pad_state = 0x00;

    ui_state.in_root_mode = false;
    ui_state.in_swing_mode = false;
    return ui_state;
}

typedef struct {
    uint8_t rocket_octave;
    uint8_t p50_octave;
    Root root;
    Scale scale;
    uint8_t ticks_counter;
    long step;
    uint8_t swing;

    ChordPattern p50_pattern;
    Bassline rocket_pattern;
    float rocket_density;
    
    GatePattern16 ac_503_pattern;
    GatePatternAB bd_503_pattern;
    GatePattern16 sd_503_pattern;
    GatePattern16 hh_503_pattern;
    GatePattern16 oh_503_pattern;

    GatePattern16 ac_522_pattern;
    GatePatternAB bd_522_pattern;
    GatePattern16 lo_tom_522_pattern;
    GatePattern16 mi_tom_522_pattern;
    GatePattern16 rs_522_pattern;
    GatePattern64 clave_522_pattern;
    GatePattern16 clap_522_pattern;
    GatePattern64 hh_522_pattern;
    GatePattern64 oh_522_pattern;
    GatePattern64 sd_522_pattern;
    
    uint8_t storage_522[16];
    uint8_t storage_503[16];
    uint8_t storage_p50[16];
    uint8_t storage_rocket[16];
    
    UiState uiState;
} ApplicationData;

#endif // DEFS_H
