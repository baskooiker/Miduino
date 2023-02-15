#pragma once

#include "core/defs.h"
#include "patterns/cv_patterns.h"

namespace Vleerhond {
class Fugue {
   public:
    // TODO: privatize
    CvPattern16 pattern;

    void randomizeFugue();
};
}  // namespace Vleerhond