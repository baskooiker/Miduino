#include "toms.h"

#include "instrument_base.h"
#include "patterns/gate_patterns.h"
#include "patterns/modulators.h"
#include "parameters.h"
#include "patterns/interval_pattern.h"

namespace Vleerhond
{
    Toms::Toms(
        Modulators& modulators_ref,
        TimeStruct& time_ref,
        const uint8_t midi_channel) :
        InstrumentBase(time_ref, true, midi_channel),
        tom_vel(modulators_ref)
    {
    }

    void Toms::randomize()
    {
        ofLogNotice("toms", "randomize()");
        last_randomized_time = millis();

        // Randomize toms
        this->tom_pattern.randomize();

        // Modulators
        uint8_t range = Rand::randui8(16);
        this->tom_vel.randomize(range, 127 - range);

        this->timing.randomize();
    }

    bool Toms::play()
    {
        if (kill)
            return false;

        // Play toms
        uint8_t tom_prob = this->tom_pattern.value(time);
        if (Utils::interval_hit(TimeDivision::Sixteenth, time)
            && tom_prob < 100)
        {
            this->midi_channel.note_on(
                NoteStruct(get_pitch(time), get_velocity()),
                time.get_shuffle_delay(this->timing)
            );
            return true;
        }
        return false;
    }

    uint8_t Toms::get_velocity()
    {
        uint8_t velocity = 100;
        this->tom_vel.value(time, velocity);
        return velocity;
    }

    uint8_t Toms::get_pitch(const TimeStruct& time)
    {
        uint8_t tom_prob = this->tom_pattern.value(time);
        return pitches.at(tom_prob % pitches.size());
    }
}