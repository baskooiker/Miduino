#pragma once

#include "defs.h"
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
        Scale(ScaleType type_, Root root_);
        void set_scale(const ScaleType type_);
        void set_root(const Root root);
        uint8_t* get_scale_notes(uint8_t& length) const;
        uint8_t* get_penta_notes() const;
        uint8_t apply_scale(uint8_t note_nr, uint8_t octave);
        uint8_t apply_scale_offset(uint8_t note_nr, uint8_t offset, uint8_t chord_step) const;
        bool contains(const uint8_t pitch) const;
        bool chord_contains(const uint8_t pitch, const uint8_t chord) const;
        uint8_t get_note(const uint8_t v) const;
        void get_available_chords_indices(uint8_t* chords, uint8_t& length) const;
        uint8_t get_ascending(const uint8_t pitch, const int8_t note_interval);
        uint8_t get_penta(const uint8_t index) const;
    };
}
