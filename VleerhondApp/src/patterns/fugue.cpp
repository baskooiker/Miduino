#include "patterns/fugue.h"

#include <algorithm>

#include "utils/rand.h"

namespace Vleerhond {
void Fugue::randomizeFugue() {
    // Randomize Fugue pattern
    static std::vector<uint8_t> pitch_opts = {0, 4, 2, 3, 6};
    this->pattern.length = 0;
    uint8_t length = Rand::randui8(64) < 32 ? 4 : 8;
    while (this->pattern.length < length) {
        std::random_shuffle(pitch_opts.begin(), pitch_opts.end());

        for (size_t i = 0; i < 5 && this->pattern.length < length; i++) {
            this->pattern.pattern.set(this->pattern.length++, pitch_opts[i]);
        }
    }
}
}  // namespace Vleerhond
