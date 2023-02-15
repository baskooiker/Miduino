#include "instruments/tonal_instruments/303_acid_bass.h"

#include "utils/rand.h"

namespace Vleerhond {
TB303Bass::TB303Bass(HarmonyStruct& harmony, TimeStruct& time)
    : Bass(harmony, time) {
    settings.p_euclid_16 = 16;
    settings.p_euclid_8 = 32;
    settings.p_interval = 0;
    settings.p_diddles = 32;

    settings.euclid_16.p_5 = 0;
    settings.euclid_16.p_6 = 0;
    settings.euclid_16.p_7 = 0;
    settings.euclid_16.p_9 = 16;
    settings.euclid_16.p_11 = 16;

    settings.diddles.p_min = .6;
    settings.diddles.p_max = .9;

    settings.default_note_length = 4;
}

void TB303Bass::randomize() {
    Bass::randomize();
    note_range_value = Rand::randui8(16, 56);
}

uint8_t TB303Bass::getVelocity() {
    if (this->accents.gate(this->time)) {
        return 127;
    }
    return 64;
}
}  // namespace Vleerhond
