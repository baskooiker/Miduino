#pragma once

#include "instrument_base.h"

namespace Vleerhond {
class Drone : public TonalInstrumentBase {
    GatePatternAB gate_pattern;
    CvPatternAB interval_pattern;

   public:
    uint8_t variable_pitch_range;

    Drone(HarmonyStruct& harmony_ref, TimeStruct& time_ref)
        : TonalInstrumentBase(
              harmony_ref, time_ref, true, MIDI_CHANNEL_BASS_DUB) {}

    void randomize() {
        ofLogNotice("drone", "randomize()");
        last_randomized_time = Utils::millis();

        uint8_t steps = Rand::randui8(5, 10);
        gate_pattern.setEuclid(16, steps);
        if (Rand::distribution(16, 16)) {
            gate_pattern.removeOne();
        }

        interval_pattern.setAll(0);
        for (int i = 0; i < 3; i++) {
            uint8_t fill_start = Rand::randui8(5, 8);
            for (int step = fill_start; step < 8; step++) {
                uint8_t rand_int = Rand::randui8(2, 5);
                switch (Rand::distribution(16, 16)) {
                    case 0:
                        rand_int = (uint8_t)NoteInterval::IntervalThird;
                        break;
                    case 1:
                        rand_int = (uint8_t)NoteInterval::IntervalFifth;
                        break;
                }
                interval_pattern.patterns[i].set(step, rand_int);
            }
        }
        switch (Rand::distribution(16, 16)) {
            case 0:
                interval_pattern.time_division = TimeDivision::Sixteenth;
                break;
            case 1:
                interval_pattern.time_division = TimeDivision::Eight;
                break;
        }
        switch (Rand::distribution(16, 16)) {
            case 0:
                interval_pattern.length = 8;
                break;
            case 1:
                interval_pattern.length = 16;
                break;
        }
    }

    bool play() {
        if (this->kill) {
            return false;
        }

        if (gate_pattern.gate(time)) {
            uint8_t chord_step = harmony.getChordStep(time).root;
            uint8_t pitch_interval = interval_pattern.value(time);
            uint8_t pitch = harmony.scale.applyScaleOffset(
                pitch_interval,
                Utils::rerange(this->variable_pitch_range, 36, 48), chord_step);
            midi_channel->noteOn(
                NoteStruct(
                    pitch, this->getVelocity(), time.ticksLeftInBar(),
                    NoteType::Tie),
                time.getShuffleDelay());
            return true;
        }

        return false;
    }
};
}  // namespace Vleerhond