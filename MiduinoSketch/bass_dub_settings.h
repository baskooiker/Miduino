#pragma once

#include "gate_patterns.h"

class BassDubSettings {
public:
    BassDubStyle style;
    NoteInterval note_interval;
    GatePatternAB hit_probs;
    uint8_t density;
    uint8_t v_pitch;
    uint8_t fugue_id;
    PitchStorage storage;

    BassDubSettings()
    {
        style = BassDubStyle::DubOctave;
        note_interval = NoteInterval::IntervalRoot;
        density = 0;
        v_pitch = 0;
    }
};
