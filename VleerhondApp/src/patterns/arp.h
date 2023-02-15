#pragma once

#include "core/defs.h"
#include "harmony/chord_struct.h"
#include "harmony/chords.h"

namespace Vleerhond {
class ArpData {
   protected:
    std::vector<uint8_t> arp_notes;

    RangeType range_type;
    uint8_t last_note;

   public:
    // TODO: Make private and create getters and setters
    uint8_t min;
    uint8_t range;
    uint8_t range_count;
    uint8_t counter;
    ArpType type;

    ArpData();
    uint8_t getClosest(
        const std::vector<uint8_t> arp_pitches, const bool include_current,
        uint8_t& last_note) const;

    uint8_t getArpPitch(uint8_t& arp_counter, uint8_t& last_note) const;

    void getArpPitchesByRange(const Scale& scale, const ChordStruct& chord);
    void getArpPitchesByCount(const Scale& scale, const ChordStruct& chord);
    uint8_t getNextArpPitch(const Scale& scale, const ChordStruct& chord);
    uint8_t getArpPitch() const;
};
}  // namespace Vleerhond