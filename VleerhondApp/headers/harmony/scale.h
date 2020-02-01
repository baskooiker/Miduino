#pragma once

#include <vector>

#include "core/defs.h"
#include "utils.h"
#include "strings.h"

namespace Vleerhond
{
    class Scale
    {
    public:
        Root root;
        ScaleType type;

        Scale();
        Scale(ScaleType type, Root root);
        void set_scale(const ScaleType type);
        void set_root(const Root root);
        std::vector<uint8_t> getScaleNotes() const;
        std::vector<uint8_t> getPentaNotes() const;
        uint8_t apply_scale(uint8_t note_nr, uint8_t octave);
        uint8_t apply_scale_offset(uint8_t note_nr, uint8_t offset, uint8_t chord_step) const;
        bool contains(const uint8_t pitch) const;
        bool chord_contains(const uint8_t pitch, const uint8_t chord) const;
        uint8_t get_note(const uint8_t v) const;
        std::vector<uint8_t> getAvailableChordsIndices() const;
        uint8_t get_ascending(const uint8_t pitch, const int8_t note_interval);
        uint8_t getPenta(const uint8_t index) const;
    };
}
