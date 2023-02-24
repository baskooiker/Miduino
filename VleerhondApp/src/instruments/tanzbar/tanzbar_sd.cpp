#include "instruments/tanzbar/tanzbar_sd.h"

namespace Vleerhond {
TanzbarMid::TanzbarMid(Modulators& modulators, TimeStruct& time)
    : Snare(modulators, time, NOTE_TANZBAR_SD) {
    settings.p_coef = 0;
    settings.p_off = 0;
    settings.p_rand_min = .95;

    this->params.push_back(CcParam(TB_SD_TUNE, 0, 16));
    this->params.push_back(CcParam(TB_SD_DTUNE, 0, 6));
    this->params.push_back(CcParam(TB_SD_SNAPPY, 16, 48));
    this->params.push_back(CcParam(TB_SD_TONE_DECAY, 16, 32));
    this->params.push_back(CcParam(TB_SD_TONE, 0, 32));
    this->params.push_back(CcParam(TB_SD_S_DECAY, 96, 127));
    this->params.push_back(CcParam(TB_SD_PITCH, 0, 32));
}
}  // namespace Vleerhond
