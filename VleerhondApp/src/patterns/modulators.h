#pragma once

#include "patterns/cv_patterns.h"

namespace Vleerhond {
class Lfo {
   public:
    uint16_t rate;    // in ticks per cycle
    uint16_t offset;  // in ticks per cycle

    Lfo();
    uint8_t value(const TimeStruct& time) const;
    void randomizeOffset();
    void randomizeSyncHigh();
    void randomizeSyncLow();
    void randomizeHigh();
    void randomizeLow();
    void randomize();
};

class Modulators {
   public:
    CvPattern16 pattern_a;
    CvPattern16 pattern_b;

    Lfo lfo_a;
    Lfo lfo_b;
    Lfo lfo_c;
    Lfo lfo_d;

    void randomize();
};

typedef enum {
    ModOff,
    ModLfoA,
    ModLfoB,
    ModLfoC,
    ModLfoD,
    ModPatA,
    ModPatB,
    ModRandom
} ModulationType;

class ModulationReceiver {
   protected:
    Modulators& modulators;

   public:
    uint8_t minimum;
    uint8_t range;
    ModulationType type;
    bool inverse;

    ModulationReceiver(Modulators& modulators_ref);
    bool value(const TimeStruct& time, uint8_t& value) const;

   protected:
    uint8_t value(const Modulators& modulators, const TimeStruct& time);

   public:
    void randomize();
    void randomize(
        const uint8_t range, const uint8_t offset, const float prob = 2.f);
};
}  // namespace Vleerhond