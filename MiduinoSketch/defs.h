
#define MIDI_CHANNEL_ROCKET 1
#define MIDI_CHANNEL_503    10
#define MIDI_CHANNEL_522    12

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
#define NOTE_522_CP_LONG  39
#define NOTE_522_CB       40
#define NOTE_522_HH       41
#define NOTE_522_OH       45
#define NOTE_522_CLAVE    42
#define NOTE_522_LO_TOM   44
#define NOTE_522_MI_TOM   46
#define NOTE_522_HI_TOM   49
#define NOTE_522_CYMBAL   48

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
#define BSP_STEP_13 32
#define BSP_STEP_14 33
#define BSP_STEP_15 34
#define BSP_STEP_16 35

#define BSP_KNOB_01 102
#define BSP_KNOB_02 103
#define BSP_KNOB_03 104
#define BSP_KNOB_04 105
#define BSP_KNOB_05 106
#define BSP_KNOB_06 107
#define BSP_KNOB_07 108
#define BSP_KNOB_08 109
#define BSP_KNOB_09 110
#define BSP_KNOB_10 111
#define BSP_KNOB_11 112
#define BSP_KNOB_12 113
#define BSP_KNOB_13 114
#define BSP_KNOB_14 115
#define BSP_KNOB_15 116
#define BSP_KNOB_16 117

#define BSP_PAD_01 52
#define BSP_PAD_02 53
#define BSP_PAD_03 54
#define BSP_PAD_04 55
#define BSP_PAD_05 56
#define BSP_PAD_06 57
#define BSP_PAD_07 58
#define BSP_PAD_08 59
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

typedef struct {
  uint8_t note;
  uint8_t min;
  uint8_t max;  
} RandomParam;

typedef struct {
    uint8_t gate_density;
} RocketSettings;

typedef struct {
    uint8_t perc_midi;
} Mfb522Settings;

typedef struct {
    uint8_t data[16];
    uint8_t length;
} RhythmPattern;

