#include "instruments/tanzbar/tanzbar_tom.h"

#include "utils/rand.h"

namespace Vleerhond {
TanzbarTom::TanzbarTom(Modulators& modulators, TimeStruct& time)
    : Toms(modulators, time),
      low_pitch(modulators),
      mid_pitch(modulators),
      high_pitch(modulators) {
    this->params.push_back(CcParam(TB_HTC_TUNE, 64, 127));
    this->params.push_back(CcParam(TB_HTC_DECAY, 64, 96));
    this->params.push_back(CcParam(TB_HTC_NOISE_ON_OFF, 0, 127));
    this->params.push_back(CcParam(TB_HTC_TOM_CONGA, 0, 127));
    this->params.push_back(CcParam(TB_MTC_TUNE, 64, 127));
    this->params.push_back(CcParam(TB_MTC_DECAY, 64, 96));
    this->params.push_back(CcParam(TB_MTC_NOISE_ON_OFF, 0, 127));
    this->params.push_back(CcParam(TB_MTC_TOM_CONGA, 0, 127));
    this->params.push_back(CcParam(TB_LTC_TUNE, 64, 127));
    this->params.push_back(CcParam(TB_LTC_DECAY, 64, 96));
    this->params.push_back(CcParam(TB_LTC_NOISE_ON_OFF, 0, 127));
    this->params.push_back(CcParam(TB_LTC_TOM_CONGA, 0, 127));
    this->params.push_back(CcParam(TB_TOM_NOISE, 0, 127));
}
void TanzbarTom::randomize() {
    ofLogNotice("tanzbar_perc", "randomize()");

    Toms::randomize();

    this->pitches.clear();

    switch (Rand::distribution(32, 32)) {
        case 0:
            pitches.push_back(NOTE_TANZBAR_LT);
            break;
        case 1:
            pitches.push_back(NOTE_TANZBAR_LC);
            break;
    }
    switch (Rand::distribution(32, 32)) {
        case 0:
            pitches.push_back(NOTE_TANZBAR_MT);
            break;
        case 1:
            pitches.push_back(NOTE_TANZBAR_MC);
            break;
    }
    switch (Rand::distribution(32, 32)) {
        case 0:
            pitches.push_back(NOTE_TANZBAR_HT);
            break;
        case 1:
            pitches.push_back(NOTE_TANZBAR_HC);
            break;
    }

    return;

    {
        uint8_t range = Rand::randui8(64, 128);
        low_pitch.randomize(range, Rand::randui8(127 - range), .5);
    }
    {
        uint8_t range = Rand::randui8(64, 128);
        mid_pitch.randomize(range, Rand::randui8(127 - range), .5);
    }
    {
        uint8_t range = Rand::randui8(64, 128);
        high_pitch.randomize(range, Rand::randui8(127 - range), .5);
    }
}
bool TanzbarTom::play() {
    uint8_t value = 0;
    if (low_pitch.value(time, value)) {
        getChannel()->sendCC(TB_LTC_TUNE, value);
    }
    if (mid_pitch.value(time, value)) {
        getChannel()->sendCC(TB_MTC_TUNE, value);
    }
    if (high_pitch.value(time, value)) {
        getChannel()->sendCC(TB_HTC_TUNE, value);
    }
    return Toms::play();
}
}  // namespace Vleerhond
