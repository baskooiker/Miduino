#pragma once

#include <stdint.h>
#include "consts.h"
#include "enums.h"

#ifndef ARDUINO
uint32_t millis();
#endif

class Coefficients {
public:
    float one;
    float two;
    float three;
    float four;
    float eights;
    float up;
    float down;
};

class RandomParam {
public:
  uint8_t note;
  uint8_t min;
  uint8_t max;  
};

class IntervalProbs 
{
public:
    uint8_t p_4;
    uint8_t p_8;
    uint8_t p_16;
    uint8_t p_32;
    uint8_t p_t8;
};

static const IntervalProbs arp_interval_probs = { 25, 25, 25, 0,  0 };

class ButtonState
{
public:
    unsigned long last_pressed;
    unsigned long last_released;
};

class UiState {
public:
    uint16_t bsp_button_state;

    ButtonState pad_state[NR_OF_PADS];
    ButtonState step_state[NR_OF_STEPS];
};
