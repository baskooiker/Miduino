#pragma once

#include "cv_patterns.h"

#define NUMBER_FUGUE_PLAYERS 4

class FugueSettings {
public:
    CvPattern16 pattern;
    FuguePlayerSettings player_settings[NUMBER_FUGUE_PLAYERS];
};
