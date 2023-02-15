#include "nord_drum_percussion.h"

#include "instrument_group.h"
#include "utils/rand.h"
#include "utils/utils.h"

namespace {
// Kick, Snare, Tom, Hi-hat.
std::vector<uint8_t> KITS = {3,  4,  6,  11, 15, 16, 20, 24, 26, 28, 35, 37,
                             45, 46, 47, 50, 58, 61, 63, 66, 69, 72, 78, 79};

// Kick, Snare, Tom1 Tom2.
std::vector<uint8_t> DRUM = {1,  2,  5,  7,  8,  10, 12, 13, 14, 31, 34,
                             36, 38, 40, 41, 42, 49, 51, 52, 54, 56, 57,
                             59, 62, 64, 65, 67, 68, 71, 73, 74, 75, 80};

// Made for 4 pads, for the standup percussionist.
std::vector<uint8_t> PERCS = {9,  17, 18, 19, 21, 23, 25, 27, 29, 30, 32,
                              33, 39, 43, 44, 48, 53, 55, 60, 70, 76, 77};
}  // namespace

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
    this->midi_channel->sendProgramChange(PERCS[Rand::randui8(PERCS.size())]);
}

uint8_t NordDrumPercussion::getVelocity() {
    return Utils::rerange(
        velocity_pattern.value(time), max_velocity - min_velocity,
        min_velocity);
}
}  // namespace Vleerhond
