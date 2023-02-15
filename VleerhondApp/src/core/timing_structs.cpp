#include "core/timing_structs.h"

#include "rand.h"

namespace Vleerhond {
void MicroTimingStruct::randomize() {
    this->delay = Rand::randui8();
    this->shuffle_off = Rand::randi8(-10, 10);
}
}  // namespace Vleerhond
