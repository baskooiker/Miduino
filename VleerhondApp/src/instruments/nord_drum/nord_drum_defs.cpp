#include "nord_drum_defs.h"

namespace Vleerhond {
namespace NordDrum {
// Kick, Snare, Tom, Hi-hat.
const std::vector<uint8_t> KITS = {3,  4,  6,  11, 15, 16, 20, 24,
                                   26, 28, 35, 37, 45, 46, 47, 50,
                                   58, 61, 63, 66, 69, 72, 78, 79};

// Kick, Snare, Tom1 Tom2.
const std::vector<uint8_t> DRUM = {1,  2,  5,  7,  8,  10, 12, 13, 14, 31, 34,
                                   36, 38, 40, 41, 42, 49, 51, 52, 54, 56, 57,
                                   59, 62, 64, 65, 67, 68, 71, 73, 74, 75, 80};

// Made for 4 pads, for the standup percussionist.
const std::vector<uint8_t> PERCS = {9,  17, 18, 19, 21, 23, 25, 27, 29, 30, 32,
                                    33, 39, 43, 44, 48, 53, 55, 60, 70, 76, 77};
}  // namespace NordDrum
}  // namespace Vleerhond
