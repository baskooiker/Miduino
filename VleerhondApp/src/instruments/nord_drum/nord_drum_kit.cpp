#include "instruments/nord_drum/nord_drum_kit.h"

#include "instruments/nord_drum/nord_drum_defs.h"
#include "utils/rand.h"

namespace Vleerhond {
NordDrumKit::NordDrumKit(
    HarmonyStruct& harmony, Modulators& modulators, TimeStruct& time)
    : InstrumentGroup(harmony, time),
      kick(modulators, time, 36),
      snare(modulators, time, 37),
      percussion(modulators, time, 39),
      hats(modulators, time, 40, 0) {
    instruments.push_back(&kick);
    instruments.push_back(&snare);
    instruments.push_back(&percussion);
    instruments.push_back(&hats);
}

void NordDrumKit::randomize() {
    for (auto& i : instruments) {
        i->randomize();
    }
    this->midi_channel->sendProgramChange(
        NordDrum::KITS[Rand::randui8(NordDrum::KITS.size())]);
}
}  // namespace Vleerhond
