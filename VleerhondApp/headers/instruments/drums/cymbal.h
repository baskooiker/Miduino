#pragma once

#include "instrument_base.h"
#include "gate_patterns.h"
#include "modulators.h"
#include "parameters.h"

namespace Vleerhond
{
    class Cymbal : public InstrumentBase
    {
    protected:
        MicroTimingStruct timing;
        ModulationReceiver cy_vel;
        uint8_t pitch;

    public:
        GatePatternAB cy_pattern;

        Cymbal(
            Modulators& modulators_ref,
            TimeStruct& time_ref) :
            InstrumentBase(time_ref, true),
            cy_vel(modulators_ref)
        {
        }

        virtual void randomize()
        {
            ofLogNotice("cymbal", "randomize()");
            last_randomized_time = millis();

            randomize_hi_seq();

            // Modulators
            uint8_t range = Rand::randui8(16, 64);
            this->cy_vel.randomize(range, 127 - range);

            timing.randomize();
        }

        virtual void randomize_hi_seq()
        {
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
        }

        virtual bool play()
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

        virtual uint8_t get_velocity()
        {
            uint8_t velocity = 100;
            this->cy_vel.value(time, velocity);
            return velocity;
        }
    };
}