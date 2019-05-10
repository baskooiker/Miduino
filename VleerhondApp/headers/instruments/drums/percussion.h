#pragma once

#include "instrument_base.h"
#include "gate_patterns.h"
#include "modulators.h"
#include "parameters.h"

namespace Vleerhond
{
    class Percussion : public InstrumentBase
    {
    protected:
        MicroTimingStruct timing;
        ModulationReceiver velocity_mod;
        GatePatternAB pattern;

        uint8_t pitch;

    public:

        Percussion(
            Modulators& modulators_ref,
            TimeStruct& time_ref) :
            InstrumentBase(time_ref, true),
            velocity_mod(modulators_ref)
        {
        }

        void randomize()
        {
            last_randomized_time = millis();

            switch (Rand::distribution(16, 16))
            {
            case 0:
                this->pattern.randomize(Rand::randf(.5, 1.0));
                break;
            case 1:
                this->pattern.set_coef_kick_pattern();
                break;
            }

            // Modulators
            uint8_t range = Rand::randui8(128, 64);
            this->velocity_mod.randomize(range, 127 - range);

            this->timing.randomize();
        }

        void play()
        {
            if (this->pattern.gate(time))
            {
                this->storage.note_on(
                    NoteStruct(pitch, get_velocity()),
                    time.get_shuffle_delay(this->timing)
                );
            }
        }
        virtual uint8_t get_velocity()
        {
            uint8_t velocity = 100;
            velocity_mod.value(time, velocity);
            return velocity;
        }
    };
}