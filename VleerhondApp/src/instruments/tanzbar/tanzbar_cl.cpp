#include "instruments/tanzbar/tanzbar_cl.h"

#include "utils/rand.h"

namespace Vleerhond {
TanzbarCl::TanzbarCl(Modulators& modulators_ref, TimeStruct& time_ref)
    : Percussion(modulators_ref, time_ref, NOTE_TANZBAR_CL),
      cl_pitch(modulators_ref) {
    this->params.push_back(CcParam(TB_CL_TUNE, 0, 127));
    this->params.push_back(CcParam(TB_CL_DECAY, 80, 127));
}
void TanzbarCl::randomize() {
    ofLogNotice("tanzbar_perc", "randomize()");
    Percussion::randomize();

    // Modulators
    uint8_t range = Rand::randui8(128, 64);
    this->cl_pitch.randomize(range, 127 - range, .3);
}
bool TanzbarCl::play() {
    uint8_t value = 0;
    if (this->cl_pitch.value(time, value)) {
        getChannel()->sendCC(TB_CL_TUNE, value);
    }
    return Percussion::play();
}
}  // namespace Vleerhond
