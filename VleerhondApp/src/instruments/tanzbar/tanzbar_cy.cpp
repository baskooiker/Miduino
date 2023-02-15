#include "instruments/tanzbar/tanzbar_cy.h"

#include "utils/rand.h"

namespace Vleerhond {
TanzbarCy::TanzbarCy(Modulators& modulators_ref, TimeStruct& time_ref)
    : Cymbal(modulators_ref, time_ref), cy_tune(modulators_ref) {
    pitch = NOTE_TANZBAR_CY;

    this->params.push_back(CcParam(TB_CY_DECAY, 64, 127));
    this->params.push_back(CcParam(TB_CY_TONE, 0, 127));
    this->params.push_back(CcParam(TB_CY_TUNE, 0, 127));
}
void TanzbarCy::randomize() {
    ofLogNotice("tanzbar_cy", "randomize()");
    Cymbal::randomize();

    // Modulators
    uint8_t range = Rand::randui8(32);
    this->cy_tune.randomize(range, 127 - range, .3);
}
bool TanzbarCy::play() {
    uint8_t value = 0;
    if (this->cy_tune.value(time, value)) {
        getChannel()->sendCC(TB_CY_TUNE, value);
    }
    return Cymbal::play();
}
}  // namespace Vleerhond
