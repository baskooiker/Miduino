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

    kick.settings.min_velocity = 96;
    kick.settings.play_ghost_notes = true;

    snare.settings.p_rand_min = 0.0;
    snare.settings.p_rand_max = 0.25;

    hats.settings.velocity_range = 64;
    hats.settings.velocity_offset = 127 - 64;
}

void NordDrumKit::randomize() {
    for (auto& i : instruments) {
        i->randomize();
    }
    this->midi_channel->sendProgramChange(
        NordDrum::KITS[Rand::randui8(NordDrum::KITS.size() - 1)] - 1);
}
}  // namespace Vleerhond
