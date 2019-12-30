#pragma once
#include "cymbal.h"

#include "instrument_base.h"
#include "patterns/gate_patterns.h"
#include "patterns/modulators.h"
#include "parameters.h"

namespace Vleerhond
{
    Cymbal::Cymbal(
        Modulators& modulators_ref,
        TimeStruct& time_ref,
        const uint8_t midi_channel) :
        InstrumentBase(time_ref, true, midi_channel),
        cy_vel(modulators_ref)
    {
    }

    void Cymbal::randomize()
    {
        ofLogNotice("cymbal", "randomize()");
        last_randomized_time = millis();

        // Randomize Cymbal
        switch (Rand::distribution(0, 16, 16))
        {
        case 0:
            // NOT USING HIS ONE!
            this->cy_pattern.set_coef_kick_pattern();
            this->cy_pattern.length = 16;
            break;
        case 1:
            this->cy_pattern.set_euclid(8, 3);
            this->cy_pattern.length = 8;
            break;
        case 2:
            this->cy_pattern.set_coef_hat_pattern();
            this->cy_pattern.length = 16;
            break;
        }

        // Modulators
        uint8_t range = Rand::randui8(16, 64);
        this->cy_vel.randomize(range, 127 - range);

        timing.randomize();
    }

    bool Cymbal::play()
    {
        // Play Cymbal
        if (this->cy_pattern.gate(time) && !kill)
        {
            this->midi_channel.note_on(
                NoteStruct(pitch, get_velocity()),
                time.get_shuffle_delay(this->timing)
            );
            return true;
        }
        return false;
    }

    uint8_t Cymbal::get_velocity()
    {
        uint8_t velocity = 100;
        this->cy_vel.value(time, velocity);
        return velocity;
    }
}