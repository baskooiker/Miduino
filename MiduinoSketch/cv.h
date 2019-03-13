#pragma once

#include "defs.h"
#include "gate.h"
#include "rhythms.h"

uint8_t apply_cv(uint8_t input, uint8_t range, int8_t offset)
{
    return (uint8_t)(MIN( MAX(((((int16_t)input) * range) / 128 + offset), 0), 127));
}
