#include "patterns/pattern_utils.h"

namespace Vleerhond
{
namespace PatternUtils
{
    void randomize_slides(GatePatternAB& slides)
    {
        slides.randomize(Rand::randf(.50f, .80f));
        slides.length = 8;
        slides.set(0, false);
    }

    void randomize_accents(GatePatternAB& accents)
    {
        accents.randomize(Rand::randf(.15f, .4f));
        accents.length = 8;
    }
}
}