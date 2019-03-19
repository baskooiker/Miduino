#pragma once

#include "gate_patterns.h"

class PolySettings
{
public:
    GatePatternAB gates_low;
    GatePatternAB gates;
    GatePatternAB tie_pattern;
    uint8_t pitch_offset;
    PolyType type;
    PitchStorage storage;

    PolySettings()
    {
        pitch_offset = 48;
        type = PolyType::PolyLow;
    }
};
