#pragma once

#include "instrument_base.h"
#include "gate_patterns.h"
#include "modulators.h"
#include "parameters.h"

namespace Vleerhond
{
    class Kick : public InstrumentBase
    {
    protected:
        MicroTimingStruct timing;
        uint8_t pitch;

    public:
        GatePatternAB bd_pattern;

        Kick(
            Modulators& modulators_ref,
            TimeStruct& time_ref) :
            InstrumentBase(time_ref, true)
        {
        }

        virtual void randomize()
        {
            ofLogNotice("kick", "randomize");
            last_randomized_time = millis();

            this->bd_pattern.set_coef_kick_pattern();

            if (Rand::distribution(32, 12))
            {
                this->randomize_kick();
            }

            this->timing.randomize();
        }

        virtual void play()
        {
            if (this->bd_pattern.gate(time) && !this->kill)
            {
                uint8_t vel = get_velocity();
                this->storage.note_on(
                    NoteStruct(this->pitch, vel),
                    time.get_shuffle_delay(this->timing)
                );
            }
        }

        virtual uint8_t get_velocity()
        {
            bool quarter_hit = Utils::interval_hit(TimeDivision::Quarter, time);
            return quarter_hit ? 127 : 63;
        }

    protected:
        virtual void randomize_kick()
        {
            // Fill in first or second half of bar
            uint8_t half = Rand::distribution(64, 64);

            uint8_t bar = this->bd_pattern.abPattern.value(Rand::randui8(4));
            this->bd_pattern.patterns[bar].set_kick_fill(half * 8);
        }

    };
}