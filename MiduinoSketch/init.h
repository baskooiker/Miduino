#pragma once

IntervalPattern init_interval_pattern()
{
    IntervalPattern pattern = { };
    for (int i = 0; i < 16; i++)
    {
        pattern.pattern[i] = TimeDivision::TIME_DIVISION_SIXTEENTH;
    }
    pattern.time_division = TimeDivision::TIME_DIVISION_EIGHT;
    return pattern;
}

ApplicationData init_application_data()
{
    ApplicationData i = { 0 };
    // TODO
    return i;
}