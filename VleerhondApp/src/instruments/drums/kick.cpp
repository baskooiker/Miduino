#include "kick.h"

#include "instrument_base.h"
#include "gate_patterns.h"
#include "modulators.h"
#include "parameters.h"

namespace Vleerhond
{
    Kick::Kick(
        Modulators& modulators_ref,
        TimeStruct& time_ref) :
        InstrumentBase(time_ref, true)
    {
    }

    void Kick::randomize()
    {
        ofLogNotice("kick", "randomize");
        last_randomized_time = millis();

        this->bd_pattern.set_coef_kick_pattern();

        if (Rand::distribution(32, 12))
        {
            this->randomize_kick();
        }
        this->accents = bd_pattern;

        this->timing.randomize();
    }

    bool Kick::play()
    {
        if (this->bd_pattern.gate(time) && !this->kill)
        {
            //ofLogNotice("kick", "play kick");
            uint8_t vel = get_velocity();
            this->midi_channel.note_on(
                NoteStruct(this->pitch, vel),
                time.get_shuffle_delay(this->timing)
            );
            return true;
        }
        return false;
    }

    uint8_t Kick::get_velocity()
    {
        return accents.gate(this->time) ? 127 : 63;
    }

    void Kick::randomize_kick()
    {
        // Fill in first or second half of bar
        uint8_t half = Rand::distribution(64, 64);

        uint8_t bar = this->bd_pattern.abPattern.value(Rand::randui8(4));
        this->bd_pattern.patterns[bar].set_kick_fill(half * 8);
    }

}