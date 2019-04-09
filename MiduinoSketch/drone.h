#pragma once

#include "instrument_base.h"

class Drone : public TonalInstrumentBase
{
    GatePatternAB gate_pattern;
    CvPatternAB interval_pattern;

public:
    uint8_t variable_pitch_range;

    Drone(HarmonyStruct& harmony_ref,
        TimeStruct& time_ref) :
        TonalInstrumentBase(harmony_ref, time_ref)
    {
    }

    void randomize()
    {
        last_randomized_time = millis();

        uint8_t steps = Rand::randui8(5, 10);
        gate_pattern.set_euclid(16, steps);
        if (Rand::distribution(16, 16))
        {
            gate_pattern.remove_one();
        }

        interval_pattern.set_all(0);
        for (int i = 0; i < 3; i++)
        {
            uint8_t fill_start = Rand::randui8(5, 8);
            for (int step = fill_start; step < 8; step++)
            {
                uint8_t rand_int = Rand::randui8(2, 5);
                interval_pattern.patterns[i].set(step, rand_int);
            }
        }
        switch (Rand::distribution(16, 16))
        {
        case 0: interval_pattern.time_division = TimeDivision::Sixteenth; break;
        case 1: interval_pattern.time_division = TimeDivision::Eight; break;
        }
        switch (Rand::distribution(16, 16))
        {
        case 0: interval_pattern.length = 8; break;
        case 1: interval_pattern.length = 16; break;
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
            uint8_t pitch_interval = interval_pattern.value(time);
            uint8_t pitch = harmony.scale.apply_scale_offset(
                pitch_interval, 
                Utils::rerange(this->variable_pitch_range, 36, 36),
                chord_step
            );
            storage.note_on(
                NoteStruct(pitch, 64, time.ticks_left_in_bar(), NoteType::Tie),
                time.get_shuffle_delay()
            );
        }

    }
};
