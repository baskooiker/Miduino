#pragma once

#include "cv_patterns.h"

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
