#include "instruments/tanzbar/tanzbar_hats.h"

#include "utils/rand.h"

namespace Vleerhond {
AbstractTanzbarHats::AbstractTanzbarHats(
    Modulators& modulators, TimeStruct& time)
    : Hats(modulators, time, NOTE_TANZBAR_HH, NOTE_TANZBAR_OH),
      tune_mod(modulators) {
    settings.p_euclid = 0;
    settings.p_drop = 0;
    settings.p_3_4 = 0;

    this->params.push_back(CcParam(TB_HH_TUNE, 0, 127));
}

bool AbstractTanzbarHats::play() {
    if (this->isKilled()) {
        return false;
    }

    uint8_t value = 0;
    if (tune_mod.value(time, value)) {
        getChannel()->sendCC(TB_HH_TUNE, value);
    }
    return Hats::play();
}

TanzbarRegularHats::TanzbarRegularHats(Modulators& modulators, TimeStruct& time)
    : AbstractTanzbarHats(modulators, time) {}

void TanzbarRegularHats::randomize() {
    Hats::randomize();
    // Modulator
    {
        uint8_t range = Rand::randui8(16, 96);
        uint8_t off = Rand::randui8(127 - range);
        this->tune_mod.randomize(range, off, .33);
    }
}

TanzbarAlternativeHats::TanzbarAlternativeHats(
    Modulators& modulators, TimeStruct& time)
    : AbstractTanzbarHats(modulators, time) {}

void TanzbarAlternativeHats::randomize() {
    int hh_length = Rand::distribute<int>({3, 5, 7});
    hh_pattern.setAll(0);
    hh_pattern.abPattern.setConst();
    hh_pattern.length = hh_length;
    uint8_t hh_count = Rand::randui8(1, hh_length / 2 + 1);
    for (uint8_t i = 0; i < hh_count; i++) {
        hh_pattern.addOneGrouped();
    }

    int oh_length = Rand::distribute<int>({5, 7, 11});
    oh_pattern.setAll(0);
    oh_pattern.abPattern.setConst();
    oh_pattern.length = oh_length;
    uint8_t oh_count = Rand::randui8(1, oh_length / 3 + 1);
    for (uint8_t i = 0; i < oh_count; i++) {
        oh_pattern.addOneGrouped();
    }

    // Modulator
    {
        uint8_t range = Rand::randui8(16, 96);
        uint8_t off = Rand::randui8(127 - range);
        this->tune_mod.randomize(range, off, .33);
    }
}
}  // namespace Vleerhond
