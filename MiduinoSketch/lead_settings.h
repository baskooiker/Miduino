#pragma once

#include "cv_patterns.h"
#include "gate_patterns.h"
#include "midi_channel.h"

class LeadSettings
{
public:
    ArpData arp_data;
    CvPatternAB min_pitch_pattern;
    GatePatternAB pattern_slow;
    LeadStyle style;
    PitchStorage storage;

    LeadSettings()
    {
        style = LeadStyle::LeadSlow;
    }
};
