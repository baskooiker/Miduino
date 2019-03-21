#pragma once

#include "defs.h"
#include "rand.h"
#include "utils.h"

bool interval_hit(const TimeDivision time_division, const TimeStruct& time)
{
    return time.tick % (uint32_t)time_division == 0;
}
