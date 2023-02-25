#include "instruments/minitaur/moog_long_bass.h"

#include <stdint.h>

#include "utils/rand.h"

namespace Vleerhond {
MoogLongBass::MoogLongBass(
    HarmonyStruct& harmony, Modulators& modulators, TimeStruct& time)
    : MoogBass(harmony, modulators, time) {
    name = "MoogLongBass";
}

void MoogLongBass::randomize() {
    MoogBass::randomize();
    ofLogNotice("MoogLongBass", "randomize()");
    this->setVariableDensity(0);
    setVariablePitch(127);

    this->style = BassStyle::BassEuclid;

    this->euclid_pattern.abPattern.setABCB();

    // Reset all patterns
    // for (size_t i = 0; i < 16; i++)
    //{
    //    euclid_pattern.patterns[0].setGate(i, false);
    //    euclid_pattern.patterns[1].setGate(i, false);
    //    euclid_pattern.patterns[2].setGate(i, false);
    //}
    euclid_pattern.patterns[0].pattern = 0;
    euclid_pattern.patterns[1].pattern = 0;
    euclid_pattern.patterns[2].pattern = 0;

    // Set rhythmic patterns
    euclid_pattern.patterns[0].setGate(0, true);
    euclid_pattern.patterns[1].setEuclid(
        Rand::distribute<int>({3, 4, 5}), 8, 8);
    euclid_pattern.patterns[2].setEuclid(Rand::distribute<int>({3, 4, 5}), 8);
}
}  // namespace Vleerhond
