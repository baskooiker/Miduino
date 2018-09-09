
#define MIDI_CHANNEL_ROCKET 1
#define MIDI_CHANNEL_503    10

#define TICKS_PER_STEP 6


#define NOTE_503_BD 36
#define NOTE_503_SD 38
#define NOTE_503_LT 41
#define NOTE_503_MT 43
#define NOTE_503_HT 45 
#define NOTE_503_CY 46
#define NOTE_503_OH 39
#define NOTE_503_HH 40

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

#define BSP_ROT_01 102
#define BSP_ROT_02 103
#define BSP_ROT_03 104
#define BSP_ROT_04 105
#define BSP_ROT_05 106
#define BSP_ROT_06 107
#define BSP_ROT_07 108
#define BSP_ROT_08 109
#define BSP_ROT_09 110
#define BSP_ROT_10 111
#define BSP_ROT_11 112
#define BSP_ROT_12 113
#define BSP_ROT_13 114
#define BSP_ROT_14 115
#define BSP_ROT_15 116
#define BSP_ROT_16 117

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

