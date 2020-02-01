#pragma once

#include "core/defs.h"
#include "harmony/chords.h"
#include "utils/rand.h"

namespace Vleerhond
{
    class ArpData
    {
    protected:
        std::vector<uint8_t> arp_notes;

        RangeType range_type;
        uint8_t last_note;

    public:
        uint8_t min;
        uint8_t range;
        uint8_t range_count;
        uint8_t counter;
        ArpType type;

        ArpData();
        uint8_t getClosest(
            const std::vector<uint8_t> arp_pitches,
            const bool include_current,
            uint8_t& last_note) const;

        uint8_t get_arp_pitch(
            uint8_t& arp_counter,
            uint8_t &last_note) const;

        void get_arp_pitches_by_range(const Scale& scale, const uint8_t chord);
        void get_arp_pitches_by_count(const Scale& scale, const uint8_t chord);
        uint8_t get_next_arp_pitch(const Scale& scale, const uint8_t chord);
        uint8_t get_arp_pitch() const;
    };
}