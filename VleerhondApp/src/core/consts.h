#pragma once

#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

#define MIDI_A_NAME "ttymidi"
#define MIDI_B_NAME "MIDISPORT 2x4 Out 2"
#define MIDI_C_NAME "MIDISPORT 2x4 Out 3"
#define MIDI_D_NAME "MIDISPORT 2x4 Out 4"

#define MIDI_BSP_IN "Arturia BeatStep Pro"
#define MIDI_BSP_OUT "Arturia BeatStep Pro"

#define MIDI_CHANNEL_TANZBAR 11
#define MIDI_CHANNEL_TB_CV1 1
#define MIDI_CHANNEL_TB_CV2 2
#define MIDI_CC_CHANNEL_TANZBAR 10
#define MIDI_CHANNEL_NEUTRON 4
#define VERMONA_CONTROL_CHANNEL 14
#define MIDI_CHANNEL_TB303 14

#define MIDI_CHANNEL_DS 13

#define COMMON_DENOMINATOR 240240  // Exept 9
#define TICKS_PER_STEP 6

#define STEPS_IN_BAR (16)
#define TICKS_IN_BAR (96)

#define MODULATION_WHEEL_CC 1

// MIDI 503
#define NOTE_503_BD 36
#define NOTE_503_SD 38
#define NOTE_503_LT 41
#define NOTE_503_MT 43
#define NOTE_503_HT 45
#define NOTE_503_CY 46
#define NOTE_503_OH 39
#define NOTE_503_HH 40

#define NOTE_503_HH_1 91
#define NOTE_503_HH_2 93
#define NOTE_503_HH_3 95

#define NOTE_503_BD_MIN 48
#define NOTE_503_BD_MAX 59
#define NOTE_503_SD_MIN 60
#define NOTE_503_SD_MAX 71
#define NOTE_503_TOM_MIN 72
#define NOTE_503_TOM_MAX 83
#define NOTE_503_CY_MIN 84
#define NOTE_503_CY_MAX 89

// MIDI 522
#define NOTE_522_BD_SHORT 35
#define NOTE_522_BD_LONG 36
#define NOTE_522_RS 37
#define NOTE_522_SN 38
#define NOTE_522_CP_SHORT 39
#define NOTE_522_CP_LONG 40
#define NOTE_522_CB 41
#define NOTE_522_HH 42
#define NOTE_522_OH 46
#define NOTE_522_CLAVE 43
#define NOTE_522_LO_TOM 45
#define NOTE_522_MI_TOM 47
#define NOTE_522_HI_TOM 48
#define NOTE_522_HI2_TOM 50
#define NOTE_522_CYMBAL 49

#define NOTE_TANZBAR_BD1 36
#define NOTE_TANZBAR_BD2 37
#define NOTE_TANZBAR_SD 38
#define NOTE_TANZBAR_RS 39
#define NOTE_TANZBAR_CY 40
#define NOTE_TANZBAR_OH 41
#define NOTE_TANZBAR_HH 42
#define NOTE_TANZBAR_CL 43
#define NOTE_TANZBAR_CP 44
#define NOTE_TANZBAR_LT 45
#define NOTE_TANZBAR_LC 46
#define NOTE_TANZBAR_MT 47
#define NOTE_TANZBAR_MC 48
#define NOTE_TANZBAR_HT 49
#define NOTE_TANZBAR_HC 50
#define NOTE_TANZBAR_CB 51
#define NOTE_TANZBAR_MA 52

// BSP CC's
#define BSP_STEP_01 24
#define BSP_STEP_02 25
#define BSP_STEP_03 26
#define BSP_STEP_04 27
#define BSP_STEP_05 28
#define BSP_STEP_06 29
#define BSP_STEP_07 30
#define BSP_STEP_08 31

#define BSP_PAD_TOP_01 87
#define BSP_PAD_TOP_02 88
#define BSP_PAD_TOP_03 89
#define BSP_PAD_TOP_04 90
#define BSP_PAD_TOP_05 91
#define BSP_PAD_TOP_06 92
#define BSP_PAD_TOP_07 93
#define BSP_PAD_TOP_08 94

#define BSP_PAD_BTM_01 48  // RIGHT_BTM
#define BSP_PAD_BTM_02 49
#define BSP_PAD_BTM_03 50
#define BSP_PAD_BTM_04 51
#define BSP_PAD_BTM_05 52
#define BSP_PAD_BTM_06 53
#define BSP_PAD_BTM_07 54
#define BSP_PAD_BTM_08 55  // SHIFT: PAD BOTTOM RIGHT

#define BSP_STEP_09 40
#define BSP_STEP_10 41
#define BSP_STEP_11 42
#define BSP_STEP_12 43
#define BSP_STEP_13 44
#define BSP_STEP_14 45
#define BSP_STEP_15 46
#define BSP_STEP_16 47

#define BSP_ROT_BTM_01 78  // BOTTOM
#define BSP_ROT_BTM_02 79
#define BSP_ROT_BTM_03 80
#define BSP_ROT_BTM_04 81
#define BSP_ROT_BTM_05 82
#define BSP_ROT_BTM_06 83
#define BSP_ROT_BTM_07 85
#define BSP_ROT_BTM_08 86

#define BSP_ROT_TOP_01 16
#define BSP_ROT_TOP_02 17
#define BSP_ROT_TOP_03 18
#define BSP_ROT_TOP_04 19
#define BSP_ROT_TOP_05 20
#define BSP_ROT_TOP_06 21
#define BSP_ROT_TOP_07 22
#define BSP_ROT_TOP_08 23

// MFB 503 CC's
#define BD_LEVEL 8
#define BD_TUNE 9
#define BD_DECAY 10
#define BD_PITCH 11
#define BD_DRIVE 12
#define BD_ATTACK 13

#define SD_LEVEL 16
#define SD_TUNE 17
#define SD_DECAY 18
#define SD_NOISE 19

#define MFB_503_LT_LEVEL 24
#define MFB_503_LT_TUNE 25
#define MFB_503_LT_DECAY 26
#define MFB_503_LT_PITCH 27

#define MFB_503_MT_LEVEL 32
#define MFB_503_MT_TUNE 33
#define MFB_503_MT_DECAY 34
#define MFB_503_MT_PITCH 35

#define MFB_503_HT_LEVEL 40
#define MFB_503_HT_TUNE 41
#define MFB_503_HT_DECAY 42
#define MFB_503_HT_PITCH 43

#define MFB_503_CY_LEVEL 48
#define MFB_503_CY_MIX 49
#define MFB_503_CY_DECAY 50
#define MFB_503_CY_TUNE 51

#define HH_LEVEL 56
#define HH_MIX 57
#define OH_DECAY 58
#define HH_DECAY 59

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

#define TB_BD1_ATTACK 2
#define TB_BD1_DECAY 64
#define TB_BD1_PITCH 65
#define TB_BD1_TUNE 3
#define TB_BD1_NOISE 4
#define TB_BD1_FILTER 5
#define TB_BD1_DIST 6
#define TB_BD1_TRIGGER 66
#define TB_BD2_DECAY 8
#define TB_BD2_TUNE 9
#define TB_BD2_TONE 10
#define TB_SD_TUNE 11
#define TB_SD_DTUNE 12
#define TB_SD_SNAPPY 13
#define TB_SD_TONE_DECAY 67
#define TB_SD_TONE 14
#define TB_SD_S_DECAY 68
#define TB_SD_PITCH 69
#define TB_RS_TUNE 88
#define TB_CY_DECAY 70
#define TB_CY_TONE 15
#define TB_CY_TUNE 71
#define TB_OH_DECAY 72
#define TB_HH_TUNE 73
#define TB_HH_DECAY 74
#define TB_CL_TUNE 16
#define TB_CL_DECAY 17
#define TB_CP_DECAY 75
#define TB_CP_FILTER 18
#define TB_CP_ATTACK 76
#define TB_CP_TRIGGER 77
#define TB_HTC_TUNE 19
#define TB_HTC_DECAY 20
#define TB_HTC_NOISE_ON_OFF 78
#define TB_HTC_TOM_CONGA 79
#define TB_MTC_TUNE 21
#define TB_MTC_DECAY 22
#define TB_MTC_NOISE_ON_OFF 80
#define TB_MTC_TOM_CONGA 81
#define TB_LTC_TUNE 23
#define TB_LTC_DECAY 24
#define TB_LTC_NOISE_ON_OFF 82
#define TB_LTC_TOM_CONGA 83
#define TB_TOM_NOISE 84
#define TB_CB_Tune 85
#define TB_CB_Decay 86
#define TB_MA_Decay 87

// Vermona
#define VERMONA_SYNTH_1_MOD_WHEEL 88
#define VERMONA_SYNTH_2_MOD_WHEEL 89
#define VERMONA_SYNTH_3_MOD_WHEEL 90
#define VERMONA_SYNTH_4_MOD_WHEEL 91
