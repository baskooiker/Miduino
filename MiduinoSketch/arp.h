#pragma once

#include "defs.h"

uint8_t get_closest(ArpData& arp_data, const Scale scale, const uint8_t chord)
{
    // TODO
    return 0;
}

uint8_t get_arp_pitch(ArpData& arp_data, const Scale scale, const uint8_t chord)
{
    // TODO
    if (arp_data.type == ArpType::CLOSEST)
    {
        return get_closest(arp_data, scale, chord);
    }

    return 0;
}

