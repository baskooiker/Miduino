#pragma once

#include "defs.h"

IntervalPattern init_interval_pattern()
{
    IntervalPattern pattern = { };
    pattern.length = 16;
    for (int i = 0; i < pattern.length; i++)
    {
        pattern.pattern[i] = TimeDivision::Sixteenth;
    }
    pattern.time_division = TimeDivision::Eight;
    return pattern;
}

CvPatternAB init_cv_pattern_ab()
{
    CvPatternAB p = { 0 };
    p.length = 64;
    p.time_division = TimeDivision::Sixteenth;
    return p;
}

CvPattern16 init_cv_pattern()
{
    CvPattern16 p = { 0 };
    p.length = 16;
    return p;
}

GatePattern16 init_gate_pattern()
{
    GatePattern16 p = { 0 };
    p.length = 16;
    return p;
}

GatePatternAB init_gate_pattern_ab()
{
    GatePatternAB pattern = { 0 };
    pattern.patterns[0] = 0x00;
    pattern.patterns[1] = 0x00;
    pattern.patterns[2] = 0x00;
    pattern.length = 64;
    pattern.time_division = TimeDivision::Sixteenth;
    return pattern;
}
