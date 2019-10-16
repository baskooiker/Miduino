#include "snare.h"

#include "instrument_base.h"
#include "gate_patterns.h"
#include "modulators.h"
#include "parameters.h"

namespace Vleerhond
{
    Snare::Snare(
        Modulators& modulators_ref,
        TimeStruct& time_ref) :
        InstrumentBase(time_ref, true)
    {
    }

    void Snare::randomize()
    {
        last_randomized_time = millis();

        switch (Rand::distribution(16, 32, 16))
        {
        case 0:
            this->pattern.set_coef_snare_pattern();
            break;
        case 1:
            this->pattern.set_all(0);
            this->pattern.set(4, true);
            this->pattern.set(12, true);
            break;
        case 2:
            this->pattern.randomize(.75);
            break;
        }

        // Timing
        this->timing.randomize();
    }

    void Snare::play_roll(const TimeStruct& time)
    {
        static TimeDivision division = TimeDivision::Sixteenth;
        if (Utils::interval_hit(TimeDivision::Sixteenth, time))
        {
            uint8_t r = Rand::randui8(16);
            if (r < 3)
            {
                division = TimeDivision::Thirtysecond;
            }
            else
            {
                division = TimeDivision::Sixteenth;
            }
        }

        if (Utils::interval_hit(division, time))
        {
            this->midi_channel.note_on(
                NoteStruct(pitch, get_velocity()),
                time.get_shuffle_delay()
            );
        }
    }

    bool Snare::play()
    {
        if (this->snare_roll)
        {
            this->play_roll(time);
        }

        // Play snare
        if (this->pattern.gate(time) && !this->kill)
        {
            this->midi_channel.note_on(
                NoteStruct(pitch, get_velocity()),
                time.get_shuffle_delay(this->timing)
            );
            return true;
        }
        return false;
    }

    uint8_t Snare::get_velocity()
    {
        // TODO: use modulator
        return 127;
    }
}