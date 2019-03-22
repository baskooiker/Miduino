#pragma once

#include "gate_patterns.h"
#include "interval_pattern.h"

class MonoSettings : public InstrumentBase
{
public:
    MonoStyle style;
    ArpData arp_data;
    IntervalPattern int_pattern;
    GatePatternAB euclid_pattern;
    IntervalPattern lead_pattern;

    uint8_t variable_pitch_offset;
    uint8_t variable_density;

    uint8_t pitch_offset;
    uint8_t fugue_id;

    PitchStorage storage;

    MonoSettings()
    {
        style = MonoStyle::MonoSixteenths;
        fugue_id = 0;
    }
};
