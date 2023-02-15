#pragma once

#include <stdint.h>

#include "harmony/chord_type.h"

namespace Vleerhond {
class ChordStruct {
   public:
    ChordStruct(uint8_t root, ChordType chord_type)
        : root(root), chord_type(chord_type) {}

    const uint8_t root;
    const ChordType chord_type;
};
}  // namespace Vleerhond
