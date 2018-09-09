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

typedef struct {
  uint8_t note;
  uint8_t min;
  uint8_t max;  
} RandomParam;

RandomParam random_503_params[] = {
    {BD_LEVEL , 100, 127},  
    {BD_TUNE  ,   0, 64 },  
    {BD_DECAY ,   0, 127},  
    {BD_PITCH ,   0, 64 },  
    {BD_DRIVE ,   0, 127},  
    {BD_ATTACK,   0, 127},  
    
    {SD_LEVEL , 100, 127},  
    {SD_TUNE  ,   0, 127},  
    {SD_DECAY ,   0, 127},  
    {SD_NOISE ,   0, 127},  
    
    {HH_LEVEL , 100, 127},  
    {HH_MIX   ,   0, 127},  
    {OH_DECAY ,   0, 127},  
    {HH_DECAY ,   0, 127},  
};

uint8_t nr_random_503_params = sizeof(random_503_params) / sizeof(RandomParam);

