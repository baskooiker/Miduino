#include "nord_drum_percussion.h"

#include "instrument_group.h"
#include "instruments/nord_drum/nord_drum_defs.h"
#include "utils/rand.h"
#include "utils/utils.h"

namespace Vleerhond {
NordDrumPercussion::NordDrumPercussion(Modulators& modulators, TimeStruct& time)
    : InstrumentBase(time), modulators(modulators) {
    name = "nord_drum";
    notes = {36, 37, 39, 40};
    densities = {0, 0, 0, 0};
    min_velocity = 64;
    max_velocity = 100;
}

bool NordDrumPercussion::play() {
    auto it = std::max_element(std::begin(densities), std::end(densities));
    uint8_t max_density = *it;
    uint16_t v = density_pattern.value(time);

    if (Utils::intervalHit(TimeDivision::Sixteenth, time)) {
        if (v < max_density) {
            uint8_t selected_drum = Rand::distribution(v / 127.0, densities);
            this->midi_channel->noteOn(
                NoteStruct(notes[selected_drum], getVelocity()),
                time.getShuffleDelay(this->timing));
            return true;
        }
    }
    return false;
}

void NordDrumPercussion::setDensity(uint8_t index, uint8_t value) {
    densities[index % 4] = value;
}

void NordDrumPercussion::randomize() {
    density_pattern.randomize();
    velocity_pattern.randomize();
    this->midi_channel->sendProgramChange(
        NordDrum::PERCS[Rand::randui8(NordDrum::PERCS.size())] - 1);
}

uint8_t NordDrumPercussion::getVelocity() {
    return Utils::rerange(
        velocity_pattern.value(time), max_velocity - min_velocity,
        min_velocity);
}
}  // namespace Vleerhond
