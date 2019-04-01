#pragma once

#include "instrument_base.h"

class Drone : public InstrumentBase
{
    GatePatternAB gate_pattern;

public:
    uint8_t variable_pitch_range;

    Drone(HarmonyStruct& harmony_ref,
        TimeStruct& time_ref) :
        InstrumentBase(harmony_ref, time_ref)
    {
    }

    void randomize()
    {
        uint8_t steps = Rand::randui8(5, 10);
        gate_pattern.set_euclid(16, steps);
        if (Rand::distribution(16, 16))
        {
            gate_pattern.remove_one();
        }
    }

    void play()
    {
        if (this->kill)
        {
            return;
        }

        if (gate_pattern.gate(time))
        {
            uint8_t chord_step = harmony.get_chord_step(time);
            uint8_t pitch = harmony.scale.apply_scale_offset(
                0, 
                Utils::rerange(this->variable_pitch_range, 36, 72),
                chord_step
            );
            storage.note_on(
                NoteStruct(pitch, 64, time.ticks_left_in_bar(), NoteType::Tie),
                time.get_shuffle_delay()
            );
        }

    }
};
