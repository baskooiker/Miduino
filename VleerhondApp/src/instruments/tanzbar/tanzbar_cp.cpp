#include "instruments/tanzbar/tanzbar_cp.h"

#include "utils/rand.h"

namespace Vleerhond {
TanzbarCp::TanzbarCp(Modulators& modulators_ref, TimeStruct& time_ref)
    : Snare(modulators_ref, time_ref, NOTE_TANZBAR_CP),
      cp_trig(modulators_ref),
      filter_mod(modulators_ref) {
    settings.p_rand = 0;

    this->params.push_back(CcParam(TB_RS_TUNE, 0, 127));
    this->params.push_back(CcParam(TB_CP_DECAY, 0, 127));
    this->params.push_back(CcParam(TB_CP_FILTER, 32, 127));
    this->params.push_back(CcParam(TB_CP_ATTACK, 0, 127));
    this->params.push_back(CcParam(TB_CP_TRIGGER, 0, 64));
}
void TanzbarCp::randomize() {
    Snare::randomize();

    // Modulators
    uint8_t range = Rand::randui8(128);
    this->cp_trig.randomize(range, 127 - range, .3);

    {
        uint8_t range = Rand::randui8(128);
        this->filter_mod.randomize(range, 127 - range, .3);
    }
}
bool TanzbarCp::play() {
    uint8_t value = 0;
    if (this->cp_trig.value(time, value)) {
        getChannel()->sendCC(TB_CP_TRIGGER, value);
    }
    if (this->filter_mod.value(time, value)) {
        getChannel()->sendCC(TB_CP_FILTER, value);
    }
    return Snare::play();
}
}  // namespace Vleerhond
