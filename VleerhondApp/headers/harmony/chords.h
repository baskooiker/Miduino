#pragma once

#include "scale.h"
#include "utils.h"
#include "patterns/ab.h"

namespace Vleerhond
{
namespace ChordUtils
{
    void get_chord_time_pattern(uint8_t* pattern, uint8_t& length); 
    void get_chord_seq(uint8_t* options, uint8_t length, uint8_t* seq);
    void get_chord(uint8_t chord, Scale& scale, uint8_t offset, uint8_t* chord_notes, uint8_t& size);
}
}