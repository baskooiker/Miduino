#include "patterns/pattern_utils.h"

namespace Vleerhond
{
namespace PatternUtils
{
    void randomizeSlides(GatePatternAB& slides)
    {
        slides.randomize(Rand::randf(.125f, .25f));
        slides.length = 16;
        slides.set(0, false);
    }

    void randomizeAccents(GatePatternAB& accents)
    {
        accents.randomize(Rand::randf(.15f, .4f));
        accents.length = 16;
    }
}
}