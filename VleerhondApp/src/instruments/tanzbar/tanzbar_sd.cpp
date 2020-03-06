#include "instruments/tanzbar/tanzbar_sd.h"

namespace Vleerhond
{
    TanzbarMid::TanzbarMid(Modulators & modulators_ref, TimeStruct & time_ref) :
        Snare(modulators_ref, time_ref)
    {
        pitch = NOTE_TANZBAR_SD;
        settings.p_coef = 0;
        settings.p_off = 0;

        this->params.push_back(CcParam(TB_SD_TUNE, 0, 16));
        this->params.push_back(CcParam(TB_SD_DTUNE, 0, 6));
        this->params.push_back(CcParam(TB_SD_SNAPPY, 32, 96));
        this->params.push_back(CcParam(TB_SD_SN_DECAY, 110, 127));
        this->params.push_back(CcParam(TB_SD_TONE, 0, 32));
        this->params.push_back(CcParam(TB_SD_TONE_DECAY, 0, 16));
        this->params.push_back(CcParam(TB_SD_PITCH, 0, 32));
    }
}
