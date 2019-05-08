#pragma once

#include "instrument_base.h"
#include "gate_patterns.h"
#include "modulators.h"
#include "parameters.h"
#include "interval_pattern.h"

namespace Vleerhond
{
    class Hats : public InstrumentBase
    {
    protected:
        MicroTimingStruct timing;
        ModulationReceiver hats_vel;
        uint8_t pitch_closed;
        uint8_t pitch_open;

    public:
        GatePatternAB hh_pattern;
        GatePatternAB oh_pattern;

        IntervalPattern hat_int_pattern;
        CvPatternAB hat_velocity;

        HatClosedStyle hat_closed_style;

        Hats(
            Modulators& modulators_ref,
            TimeStruct& time_ref) :
            InstrumentBase(time_ref, true),
            hats_vel(modulators_ref)
        {
            hat_closed_style = HatClosedStyle::HatClosedRegular;
            randomize();
        }

        virtual void randomize()
        {
            ofLogNotice("hats", "randomize()");
            last_randomized_time = millis();

            randomize_seq();

            this->timing.randomize();

            uint8_t range = Rand::randui8(16, 64);
            this->hats_vel.randomize(range, 127 - range);

        }

        virtual void randomize_seq()
        {
            // Randomize hats
            this->oh_pattern.set_coef_hat_pattern();
            uint8_t four_pat = 0;
            switch (Rand::distribution(32, 10, 10, 10, 10))
            {
            case 0: four_pat = BXXXX; break;
            case 1: four_pat = BXXX0; break;
            case 2: four_pat = BXX0X; break;
            case 3: four_pat = BX0XX; break;
            case 4: four_pat = B0XXX; break;
            }
            for (int i = 0; i < 3; i++)
            {
                for (int step = 0; step < 4; step++)
                {
                    this->hh_pattern.patterns[i].set_gate(step, Utils::gate(four_pat, step));
                }
                this->hh_pattern.length = 4;
                this->hh_pattern.abPattern.set_ab_pattern();
            }
            switch (Rand::distribution(32, 32))
            {
            case 0: this->hat_closed_style = HatClosedStyle::HatClosedRegular; break;
            case 1: this->hat_closed_style = HatClosedStyle::HatClosedInterval; break;
            }

            this->hat_int_pattern.randomize_interval_hat();
            this->hat_velocity.randomize();
        }

        virtual void play_hats_closed()
        {
            switch (this->hat_closed_style)
            {
            case HatClosedStyle::HatClosedInterval:
            {
                TimeDivision div = this->hat_int_pattern.interval(time);
                if (Utils::interval_hit(div, time))
                {
                    uint8_t shuffle_delay = 0;
                    if (div < TimeDivision::Sixteenth)
                    {
                        shuffle_delay = time.get_shuffle_delay(this->timing);
                    }
                    this->storage.note_on(
                        NoteStruct(pitch_closed, get_velocity()),
                        shuffle_delay
                    );
                }
                break;
            }
            case HatClosedStyle::HatClosedRegular:
                if (this->hh_pattern.gate(time))
                {
                    this->storage.note_on(
                        NoteStruct(pitch_closed, get_velocity()),
                        time.get_shuffle_delay(this->timing)
                    );
                }
                break;
            }
        }

        virtual bool play_hats_open()
        {
            if (this->kill)
                return false;

            if (this->oh_pattern.gate(time))
            {
                this->storage.note_on(
                    NoteStruct(pitch_open, get_velocity()),
                    time.get_shuffle_delay(this->timing)
                );
                return true;
            }
            return false;
        }

        virtual void play()
        {
            if (this->kill)
                return;

            if (!play_hats_open())
            {
                play_hats_closed();
            }
        }

        virtual uint8_t get_velocity()
        {
            uint8_t velocity = 63;
            if ((time.tick / TICKS_PER_STEP) % 4 == 2)
            {
                velocity = 127;
            }
            else
            {
                velocity = Utils::rerange(this->hat_velocity.value(time), 50, 32);
            }
            return velocity;
        }
    };
}