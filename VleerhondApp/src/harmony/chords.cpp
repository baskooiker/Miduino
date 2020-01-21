#include "harmony/chords.h"

#include "harmony/scale.h"
#include "patterns/ab.h"

#include "utils.h"

#include <vector>

namespace Vleerhond
{
namespace ChordUtils
{
    static const std::vector<std::vector<uint8_t>> chord_time_patterns_short = {
        {8,  8},
        {12, 4},
    };

    static const std::vector<std::vector<uint8_t>> chord_time_patterns_long = {
        {8,  4, 4},
        {6,  6, 4},
        {4,  4, 4, 4},
        {6,  2, 6, 2},
    };

    std::vector<uint8_t> getChordTimePatternShort()
    {
        return chord_time_patterns_short[Rand::randui8(chord_time_patterns_short.size())];
    }

    std::vector<uint8_t> getChordTimePatternLong()
    {
        return chord_time_patterns_long[Rand::randui8(chord_time_patterns_long.size())];
    }

    std::vector<uint8_t> getChordSeq(std::vector<uint8_t> options, uint8_t length)
    {
        std::vector<uint8_t> seq;
        if (length == 4 && Rand::distribution(16, 0) == 1)
        {
            AbPattern ab_pat;
            ab_pat.set_ab_pattern_high();
            for (int i = 0; i < 4; i++)
            {
                seq.push_back(options[ab_pat.value(i)]);
            }
        }
        else
        {
            for (int i = 0; i < length; i++)
            {
                seq.push_back(options[i]);
            }
        }
        return seq;
    }

    std::vector<uint8_t> getChord(uint8_t chord, Scale& scale, uint8_t offset)
    {
        std::vector<uint8_t> chord_notes;
        for (int i = offset; i < offset + 12; i++)
        {
            if (scale.chord_contains(i, chord))
            {
                chord_notes.push_back(i);
            }
        }
        return chord_notes;
    }
}
}
