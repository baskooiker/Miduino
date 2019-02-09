#pragma once

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define CLIP(value,minimum,maximum) (MIN(MAX(value, minimum), maximum))

#define MIDI_CHANNEL_BASS     1
#define MIDI_CHANNEL_BASS_DUB 4
#define MIDI_CHANNEL_MONO     2
#define MIDI_CHANNEL_MONO_2   3
#define MIDI_CHANNEL_POLY     6
#define MIDI_CHANNEL_LEAD     7
#define MIDI_CHANNEL_503      10
#define MIDI_CHANNEL_522      12

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

#define NOTE_503_BD_MIN 48
#define NOTE_503_BD_MAX 59
#define NOTE_503_TOM_MIN 72
#define NOTE_503_TOM_MAX 83
#define NOTE_503_CY_MIN 84
#define NOTE_503_CY_MAX 89

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

#define MFB_503_LT_LEVEL 24
#define MFB_503_LT_TUNE  25
#define MFB_503_LT_DECAY 26
#define MFB_503_LT_PITCH 27

#define MFB_503_MT_LEVEL 32
#define MFB_503_MT_TUNE  33
#define MFB_503_MT_DECAY 34
#define MFB_503_MT_PITCH 35

#define MFB_503_HT_LEVEL 40
#define MFB_503_HT_TUNE  41
#define MFB_503_HT_DECAY 42
#define MFB_503_HT_PITCH 43

#define MFB_503_CY_LEVEL 48
#define MFB_503_CY_MIX   49
#define MFB_503_CY_DECAY 50
#define MFB_503_CY_TUNE  51

#define HH_LEVEL  56
#define HH_MIX    57
#define OH_DECAY  58
#define HH_DECAY  59

#define TOM_DEC_MIN 64
#define TOM_DEC_MAX 96

#define B0000 0
#define B000X 1
#define B00X0 2
#define B00XX 3
#define B0X00 4
#define B0X0X 5
#define B0XX0 6
#define B0XXX 7
#define BX000 8
#define BX00X 9
#define BX0X0 10
#define BX0XX 11
#define BXX00 12
#define BXX0X 13
#define BXXX0 14
#define BXXXX 15
