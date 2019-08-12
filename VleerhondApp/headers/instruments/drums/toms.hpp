#pragma once

#include "instrument_base.hpp"
#include "gate_patterns.hpp"
#include "modulators.hpp"
#include "parameters.hpp"
#include "interval_pattern.hpp"

namespace Vleerhond
{
    class Toms : public InstrumentBase
    {
    protected:
        MicroTimingStruct timing;

        ModulationReceiver tom_vel;

        CvPatternAB tom_pattern;

        GatePatternAB tom_mask;
        std::vector<uint8_t> pitches;

    public:

        Toms(
            Modulators& modulators_ref,
            TimeStruct& time_ref) :
            InstrumentBase(time_ref, true),
            tom_vel(modulators_ref)
        {
        }

        virtual void randomize()
        {
            ofLogNotice("toms", "randomize()");
            last_randomized_time = millis();

            // Randomize toms
            this->tom_pattern.randomize();
            this->tom_mask.randomize_mask_pattern();

            // Modulators
            uint8_t range = Rand::randui8(16, 32);
            this->tom_vel.randomize(range, 127 - range);

            this->timing.randomize();
        }

        virtual bool play()
        {
            // Play toms
            uint8_t tom_prob = this->tom_pattern.value(time);
            if (time.interval_hit(TimeDivision::Sixteenth)
                && tom_prob < 100
                && this->tom_mask.gate(time)
                && !kill)
            {
                this->midi_channel.note_on(
                    NoteStruct(get_pitch(time), get_velocity()),
                    time.get_shuffle_delay(this->timing)
                );
                return true;
            }
            return false;
        }

        virtual uint8_t get_velocity()
        {
            uint8_t velocity = 100;
            this->tom_vel.value(time, velocity);
            return velocity;
        }

        virtual uint8_t get_pitch(const TimeStruct& time)
        {
            uint8_t tom_prob = this->tom_pattern.value(time);
            return pitches.at(tom_prob % pitches.size());
        }

    };
}