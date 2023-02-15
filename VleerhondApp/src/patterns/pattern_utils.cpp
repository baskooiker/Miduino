#include "patterns/pattern_utils.h"

#include "utils/rand.h"

namespace Vleerhond {
namespace PatternUtils {
void randomizeSlides(GatePatternAB& slides) {
    slides.randomize(Rand::randf(.25f, .5f));
    slides.length = 16;
    slides.set(0, false);
}

void randomizeAccents(GatePatternAB& accents) {
    accents.randomize(Rand::randf(.15f, .4f));
    accents.length = 16;
}
}  // namespace PatternUtils
}  // namespace Vleerhond