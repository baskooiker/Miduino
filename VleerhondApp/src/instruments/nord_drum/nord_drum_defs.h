#pragma once

#include <stdint.h>

#include <vector>

namespace Vleerhond {
namespace NordDrum {
// Kick, Snare, Tom, Hi-hat.
extern const std::vector<uint8_t> KITS;

// Kick, Snare, Tom1 Tom2.
extern const std::vector<uint8_t> DRUM;

// Made for 4 pads, for the standup percussionist.
extern const std::vector<uint8_t> PERCS;
}  // namespace NordDrum
}  // namespace Vleerhond
