#include "snare.h"

#include "instrument_base.h"
#include "patterns/gate_patterns.h"
#include "patterns/modulators.h"
#include "parameters.h"

namespace Vleerhond
{
    Snare::Snare(
        Modulators& modulators_ref,
        TimeStruct& time_ref) :
        InstrumentBase(time_ref)
    {
    }

    void Snare::randomize()
    {
        last_randomized_time = Utils::millis();

        switch (Rand::distribution(
            settings.p_coef, 
            settings.p_off, 
            settings.p_rand))
        {
        case 0: // Coef
            //ofLogNotice("", "randomize snare coef");
            this->pattern.set_coef_snare_pattern();
            break;
        case 1: // Off
            //ofLogNotice("", "randomize snare off");
            this->pattern.set_all(0);
            this->pattern.set(4, true);
            this->pattern.set(12, true);
            break;
        case 2: // Random
            //ofLogNotice("", "randomize snare rand");
            this->pattern.randomize(Rand::randf(settings.p_rand_min, settings.p_rand_max));
            switch (Rand::distribution(settings.p_length_8, settings.p_length_16))
            {
            case 0: pattern.length = 8; break;
            case 1: pattern.length = 16; break;
            }
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
            this->midi_channel->note_on(
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
        if (this->pattern.gate(time) && !this->isKilled())
        {
            this->midi_channel->note_on(
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

    void Snare::printPattern()
    {
        ofLogNotice("", "Snare pattern\n%s", pattern.toString());
    }
}