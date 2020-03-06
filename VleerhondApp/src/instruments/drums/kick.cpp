#include "kick.h"

#include "instrument_base.h"
#include "patterns/gate_patterns.h"
#include "patterns/modulators.h"

namespace Vleerhond
{
    Kick::Kick(
        Modulators& modulators_ref,
        TimeStruct& time_ref) :
        InstrumentBase(time_ref)
    {
    }

    void Kick::randomize()
    {
        ofLogNotice("kick", "randomize");
        InstrumentBase::randomize();
        
        bd_pattern.set_all(false);
        for (int i : {0, 4, 8, 12})
        {
            bd_pattern.set(i, true);
        }

        switch (Rand::distribution(16, 16, 64))
        {
        case 0:
        {
            this->randomize_kick();
            break;
        }
        case 1:
        {
            Coefficients ghost_coef = { 0 };
            ghost_coef.eights = Rand::randf(.0625);
            ghost_coef.up = Rand::randf(.0625);
            ghost_coef.down = Rand::randf(.0625);
            this->ghost_notes.set_coef_pattern(ghost_coef);
            break;
        }
        case 2:
            break;
        }

        this->timing.randomize();
    }

    bool Kick::play()
    {
        if (this->bd_pattern.gate(time) && !this->isKilled())
        {
            this->midi_channel->note_on(
                NoteStruct(this->pitch, get_velocity()),
                time.get_shuffle_delay(this->timing)
            );
            return true;
        }
        return false;
    }

    uint8_t Kick::get_velocity()
    {
        return this->bd_pattern.gate(time) ? 127 : 32;
    }

    void Kick::randomize_kick()
    {
        // Fill in first or second half of bar
        uint8_t half = Rand::distribution(64, 64);

        //uint8_t bar = this->bd_pattern.abPattern.value(Rand::randui8(4));
        // Always randomize the B part of the pattern
        this->bd_pattern.patterns[1].set_kick_fill(half * 8);
    }

}