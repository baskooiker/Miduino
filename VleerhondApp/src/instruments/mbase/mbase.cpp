#include "mbase.h"

namespace Vleerhond {
MBase::MBase(Modulators& modulators, TimeStruct& time)
    : Kick(modulators, time, 36) {
    this->params.push_back(CcParam(MBASE_TUNE, 64 - 10, 64 + 10));
    this->params.push_back(CcParam(MBASE_PITCH, 64 - 10, 64 + 10));
    this->params.push_back(CcParam(MBASE_DECAY, 64 - 10, 64 + 10));
    this->params.push_back(CcParam(MBASE_HARMONICS, 0, 16));
    this->params.push_back(CcParam(MBASE_PULSE, 0, 32));
    this->params.push_back(CcParam(MBASE_NOISE, 0, 32));
    this->params.push_back(CcParam(MBASE_ATTACK, 0, 32));
    // this->params.push_back(CcParam(MBASE_EQ, 0, 127));
    // this->params.push_back(CcParam(MBASE_COMPRESSION, 0, 127));
    // this->params.push_back(CcParam(MBASE_GATE_TIME, 0, 127));
    this->params.push_back(CcParam(MBASE_METAL_NOISE, 0, 32));
    this->params.push_back(CcParam(MBASE_VOLUME, 126, 127));
    this->params.push_back(CcParam(MBASE_LFO_SYNC, 126, 127));
    this->params.push_back(CcParam(MBASE_LFO_ONE_SHOT, 0, 1));
    this->params.push_back(CcParam(MBASE_LFO_INTENSITY, 0, 32));
    this->params.push_back(CcParam(MBASE_LFO_RATE, 0, 64));
}
}  // namespace Vleerhond