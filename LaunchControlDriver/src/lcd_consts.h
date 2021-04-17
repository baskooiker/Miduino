#pragma once

#include "ofLog.h"

namespace Vleerhond
{
    static const uint8_t OFF_OFF = 12;
    static const uint8_t RED_LOW = 13;
    static const uint8_t RED_FULL = 15;
    static const uint8_t AMBER_LOW = 29;
    static const uint8_t AMBER_FULL = 63;
    static const uint8_t YELLOW_FULL = 62;
    static const uint8_t GREEN_LOW = 28;
    static const uint8_t GREEN_FULL = 60;

    const char MODULE[] = "LaunchControlDriver";

    static const uint8_t CHANNEL = 1;
    static const uint8_t BUTTON_0 = 9;
    static const uint8_t BUTTON_1 = 10;
    static const uint8_t BUTTON_2 = 11;
    static const uint8_t BUTTON_3 = 12;
    static const uint8_t BUTTON_4 = 25;
    static const uint8_t BUTTON_5 = 26;
    static const uint8_t BUTTON_6 = 27;
    static const uint8_t BUTTON_7 = 28;
    static const uint8_t BUTTONS[] = {BUTTON_0, BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4, BUTTON_5, BUTTON_6, BUTTON_7};
    static const uint8_t ENCODER_A_1 = 21;
    static const uint8_t ENCODER_A_2 = 22;
    static const uint8_t ENCODER_A_3 = 23;
    static const uint8_t ENCODER_A_4 = 24;
    static const uint8_t ENCODER_A_5 = 25;
    static const uint8_t ENCODER_A_6 = 26;
    static const uint8_t ENCODER_A_7 = 27;
    static const uint8_t ENCODER_A_8 = 28;
    static const uint8_t ENCODERS_A[] = {ENCODER_A_1, ENCODER_A_2, ENCODER_A_3, ENCODER_A_4, ENCODER_A_5, ENCODER_A_6, ENCODER_A_7, ENCODER_A_8};
    static const uint8_t ENCODER_B_1 = 41;
    static const uint8_t ENCODER_B_2 = 42;
    static const uint8_t ENCODER_B_3 = 43;
    static const uint8_t ENCODER_B_4 = 44;
    static const uint8_t ENCODER_B_5 = 45;
    static const uint8_t ENCODER_B_6 = 46;
    static const uint8_t ENCODER_B_7 = 47;
    static const uint8_t ENCODER_B_8 = 48;
    static const uint8_t ENCODERS_B[] = {ENCODER_B_1, ENCODER_B_2, ENCODER_B_3, ENCODER_B_4, ENCODER_B_5, ENCODER_B_6, ENCODER_B_7, ENCODER_B_8};
    static const uint8_t UP = 114;
    static const uint8_t DOWN = 115;
    static const uint8_t LEFT = 116;
    static const uint8_t RIGHT = 117;

    enum ControlMode
    {
        CHORD_MODE,
        PATTERN_MODE
    };

}
