#include "instruments/tanzbar/tanzbar_rs.h"

#include "utils/rand.h"

namespace Vleerhond {
TanzbarRs::TanzbarRs(Modulators& modulators_ref, TimeStruct& time_ref)
    : Percussion(modulators_ref, time_ref, NOTE_TANZBAR_RS),
      rs_tune(modulators_ref) {
    this->params.push_back({TB_RS_TUNE, 0, 127});
}

void TanzbarRs::randomize() {
    Percussion::randomize();

    // Modulators
    uint8_t range = Rand::randui8(128);
    this->rs_tune.randomize(range, 127 - range, .3);
}

bool TanzbarRs::play() {
    uint8_t value = 0;
    if (this->rs_tune.value(time, value)) {
        getChannel()->sendCC(TB_RS_TUNE, value);
    }
    return Percussion::play();
}
}  // namespace Vleerhond
