#pragma once

#include <vector>

#include "core/defs.h"
#include "utils.h"
#include "string_utils.h"

#include "harmony/chord_struct.h"

namespace Vleerhond
{
    class Scale
    {
    public:
        Root root;
        ScaleType type;

        Scale();
        Scale(ScaleType type, Root root);
        void setScale(const ScaleType type);
        void setRoot(const Root root);
        std::vector<uint8_t> getScaleNotes() const;
        std::vector<uint8_t> getPentaNotes() const;
        uint8_t applyScaleOffset(uint8_t note_nr, uint8_t octave);
        uint8_t applyScaleOffset(uint8_t note_nr, uint8_t offset, uint8_t chord_step) const;
        bool contains(const uint8_t pitch) const;
        bool chordContains(const uint8_t pitch, const ChordStruct& chord) const;
        uint8_t getNote(const uint8_t v) const;
        std::vector<uint8_t> getAvailableChordsIndices() const;
        uint8_t getAscending(const uint8_t pitch, const int8_t note_interval);
        uint8_t getPenta(const uint8_t index) const;
    };
}
