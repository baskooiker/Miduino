#pragma once

#include "core/defs.h"
#include "instruments/instrument_base.h"
#include "patterns/modulators.h"
#include "patterns/gate_patterns.h"
#include "instruments/drums/snare.h"

namespace Vleerhond
{
    class TanzbarMid : public Snare
    {
    public:
        TanzbarMid(
            Modulators& modulators_ref,
            TimeStruct& time_ref);
    };
}