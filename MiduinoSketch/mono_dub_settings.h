#pragma once

#include "mono_settings.h"

class MonoDubSettings : public InstrumentBase
{
public:
    uint8_t variable_pitch_offset;
    MonoSettings settings;
    MonoDubStyle style;

    MonoDubSettings()
    {
        variable_pitch_offset = 0;
        style = MonoDubStyle::MonoDubLead;
    }
};
