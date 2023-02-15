#include "acid_bass.h"

#include "midi/midi_io.h"
#include "utils/rand.h"

namespace Vleerhond {
AcidBass::AcidBass(HarmonyStruct& harmony, TimeStruct& time)
    : Bass(harmony, time) {
    getChannel()->sendCC(VERMONA_SYNTH_1_MOD_WHEEL, 127);

    settings.p_euclid_16 = 16;
    settings.p_euclid_8 = 0;
    settings.p_interval = 0;
    settings.p_diddles = 32;

    settings.euclid_16.p_5 = 0;
    settings.euclid_16.p_6 = 0;
    settings.euclid_16.p_7 = 0;
    settings.euclid_16.p_9 = 16;
    settings.euclid_16.p_11 = 16;

    settings.diddles.p_min = .6;
    settings.diddles.p_max = .9;
}

void AcidBass::randomize() {
    Bass::randomize();
    note_range_value = Rand::randui8(16, 56);
    accent_amount = Rand::randui8(64, 96);
}

bool AcidBass::play() {
    if (isKilled()) return false;

    if (this->getHit(this->getVariableDensity(), time)) {
        uint8_t value = 0;
        if (this->accents.gate(this->time)) {
            value = accent_amount;
        }
        getChannel()->sendCC(MODULATION_WHEEL_CC, value);
    }
    return Bass::play();
}
}  // namespace Vleerhond
