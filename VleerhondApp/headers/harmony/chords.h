#pragma once

#include "scale.h"
#include "utils.h"
#include "patterns/ab.h"

namespace Vleerhond
{
namespace ChordUtils
{
    std::vector<uint8_t> getChordTimePatternShort();
    std::vector<uint8_t> getChordTimePatternLong();
    std::vector<uint8_t> getChordSeq(std::vector<uint8_t> options, uint8_t length);
    std::vector<uint8_t> getChord(uint8_t chord, Scale& scale, uint8_t offset);
}
}