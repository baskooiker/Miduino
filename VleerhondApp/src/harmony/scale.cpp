#include "harmony/scale.h"

#include "core/defs.h"
#include "utils/utils.h"
#include "strings.h"

namespace Vleerhond
{
    const std::vector<uint8_t> ionian = { 0, 2, 4, 5, 7, 9, 11 }; // 1 2  3 4 5 6   7
    const std::vector<uint8_t> ionian_penta = { 0, 2, 4,    7, 9 }; // 1 2  3 4 5 6   7
    const std::vector<uint8_t> dorian = { 0, 2, 3, 5, 7, 9, 10 }; // 1 2 b3 4 5 6  b7
    const std::vector<uint8_t> dorian_penta = { 0,    3, 5, 7,    10 }; // 1 2 b3 4 5 6  b7
    const std::vector<uint8_t> aeolian = { 0, 2, 3, 5, 7, 8, 10 }; // 1 2 b3 4 5 b6 b7 Natural Minor
    const std::vector<uint8_t> aeolian_penta = { 0,    3, 5, 7,    10 }; // 1 2 b3 4 5 b6 b7 Natural Minor
    
    Scale::Scale()
    {
        set_scale(ScaleType::AEOLIAN);
        root = Root::ROOT_C;
    }

    Scale::Scale(ScaleType type_, Root root_)
    {
        set_scale(type_);
        root = root_;
    }

    void Scale::set_scale(const ScaleType type_)
    {
        ofLogNotice("scale", "set_scale(%s)", Strings::get_string(type_).c_str());
        this->type = type_;
    }

    void Scale::set_root(const Root root)
    {
        ofLogNotice("scale", "set_scale(%s)", Strings::get_string(root).c_str());
        this->root = root;
    }

    std::vector<uint8_t> Scale::getScaleNotes() const
    {
        switch (this->type)
        {
        default:
        case ScaleType::AEOLIAN: return aeolian;
        case ScaleType::DORIAN: return dorian;
        case ScaleType::IONIAN: return ionian;
        }
    }

    std::vector<uint8_t> Scale::getPentaNotes() const
    {
        switch (this->type)
        {
        default:
        case ScaleType::AEOLIAN: return aeolian_penta;
        case ScaleType::DORIAN: return dorian_penta;
        case ScaleType::IONIAN: return ionian_penta;
        }
    }

    uint8_t Scale::apply_scale(uint8_t note_nr, uint8_t octave)
    {
        std::vector<uint8_t> notes = getScaleNotes();
        uint8_t note = notes[note_nr % notes.size()]; // Actual pitch
        note += (uint8_t)root; // Transpose to correct root note
        note += octave * 12; // Transpose octaves
        return note;
    }

    uint8_t Scale::apply_scale_offset(uint8_t note_nr, uint8_t offset, uint8_t chord_step) const
    {
        std::vector<uint8_t> notes = getScaleNotes();
        uint8_t octave = note_nr / notes.size();
        uint8_t note = notes[(note_nr + chord_step) % notes.size()]; // Actual pitch
        note += (uint8_t)root; // Transpose to correct root note
        note = Utils::clip_pitch(note, offset, 127);
        note += octave * 12; // Transpose octaves
        return note;
    }

    bool Scale::contains(const uint8_t pitch) const
    {
        return Utils::isInSet((pitch + 12 - (uint8_t)this->root) % 12, getScaleNotes());
    }

    bool Scale::chord_contains(const uint8_t pitch, const ChordStruct& chord) const
    {
        std::vector<uint8_t> notes = getScaleNotes();
        for (int i : indicesForChordType(chord.chord_type))
        {
            uint8_t note = ((pitch + 12 - (uint8_t)this->root) % 12);
            uint8_t chord_note = notes[(i + chord.root) % notes.size()];
            if (note == chord_note)
                return true;
        }
        return false;
    }

    uint8_t Scale::get_note(const uint8_t v) const
    {
        std::vector<uint8_t> notes = getScaleNotes();
        return notes[v % notes.size()];
    }

    std::vector<uint8_t> Scale::getAvailableChordsIndices() const
    {
        std::vector<uint8_t> chords;
        std::vector<uint8_t> notes = this->getScaleNotes();
        for (int i = 0; i < notes.size(); i++)
        {
            if (this->contains(notes[i] + 7))
            {
                chords.push_back(i);
            }
        }
        return chords;
    }

    uint8_t Scale::get_ascending(const uint8_t pitch, const int8_t note_interval)
    {
        uint8_t rv = pitch;
        int8_t steps = note_interval;
        int8_t sign = note_interval >= 0 ? 1 : -1;
        for (int i = pitch + sign; i < 128 && i >= 0; i++)
        {
            if (this->contains(i))
            {
                if (steps > 0)
                {
                    steps -= sign;
                }
                if (steps == 0)
                {
                    return i;
                }
            }
        }
        return rv;
    }

    uint8_t Scale::getPenta(const uint8_t index) const
    {
        std::vector<uint8_t> notes = getPentaNotes();
        return (notes[index % notes.size()] + (uint8_t)root) % 12;
    }
}
