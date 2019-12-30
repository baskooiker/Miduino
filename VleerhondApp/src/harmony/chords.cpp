#include "harmony/chords.h"

#include "harmony/scale.h"
#include "patterns/ab.h"

#include "utils.h"

namespace Vleerhond
{
namespace ChordUtils
{
    static const std::vector<std::vector<uint8_t>> chord_time_patterns = {
        {8,  8, 0, 0},
        {12, 4, 0, 0},
        {8,  4, 4, 0},
        {6,  6, 4, 0},
        {4,  4, 4, 4},
        {6,  2, 6, 2},
    };

    void get_chord_time_pattern(uint8_t* pattern, uint8_t& length)
    {
        uint8_t pat_idx = Rand::randui8(chord_time_patterns.size());
        length = 0;

        for (int i = 0; i < 4; i++)
        {
            if (chord_time_patterns[pat_idx][i] > 0)
            {
                length++;
            }
            pattern[i] = chord_time_patterns[pat_idx][i];
        }
    }

    void get_chord_seq(uint8_t* options, uint8_t length, uint8_t* seq)
    {
        if (length == 4)
        {
            AbPattern ab_pat;
            ab_pat.set_ab_pattern_high();
            for (int i = 0; i < 4; i++)
            {
                seq[i] = options[ab_pat.value(i)];
            }
        }
        else
        {
            for (int i = 0; i < 4; i++)
            {
                seq[i] = options[i];
            }
        }
    }

    void get_chord(uint8_t chord, Scale& scale, uint8_t offset, uint8_t* chord_notes, uint8_t& size)
    {
        size = 0;
        for (int i = offset; i < offset + 12; i++)
        {
            if (scale.chord_contains(i, chord))
            {
                chord_notes[size++] = i;
            }
        }
    }
}
}