#include "instruments/tanzbar/tanzbar_cb.h"

#include "utils/rand.h"

namespace Vleerhond {
TanzbarCb::TanzbarCb(Modulators& modulators_ref, TimeStruct& time_ref)
    : Percussion(modulators_ref, time_ref, NOTE_TANZBAR_CB),
      cb_tune(modulators_ref) {
    this->params.push_back(CcParam(TB_CB_Tune, 0, 127));
    this->params.push_back(CcParam(TB_CB_Decay, 0, 64));
}
void TanzbarCb::randomize() {
    ofLogNotice("tanzbar_perc", "randomize()");
    Percussion::randomize();

    // Modulators
    uint8_t range = Rand::randui8(128);
    this->cb_tune.randomize(range, 127 - range, .3);
}
bool TanzbarCb::play() {
    uint8_t value = 0;
    if (this->cb_tune.value(time, value)) {
        getChannel()->sendCC(TB_CB_Tune, value);
    }
    return Percussion::play();
}
}  // namespace Vleerhond
