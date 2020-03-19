#include "hats.h"

#include "instrument_base.h"
#include "patterns/gate_patterns.h"
#include "patterns/modulators.h"
#include "patterns/interval_pattern.h"

namespace Vleerhond
{
    Hats::Hats(
        Modulators& modulators_ref,
        TimeStruct& time_ref) :
        InstrumentBase(time_ref),
        hats_vel(modulators_ref)
    {
        hat_closed_style = HatClosedStyle::HatClosedRegular;
    }

    void Hats::randomize()
    {
        ofLogNotice("hats", "randomize()");
        InstrumentBase::randomize();

        randomize_seq();

        this->timing.randomize();

        uint8_t range = Rand::randui8(16, 64);
        this->hats_vel.randomize(range, 127 - range);

    }

    void Hats::randomize_seq()
    {
        // Randomize hats
        switch (Rand::distribution(
            settings.p_off,
            settings.p_euclid,
            settings.p_drop
        ))
        {
        case 0:
            this->oh_pattern.setCoefHatPattern();
            oh_pattern.length = 16;
            break;
        case 1:
            oh_pattern.setEuclid(8, 3);
            oh_pattern.length = 8;
            oh_pattern.abPattern.setConst(0);
            break;
        case 2:
            oh_pattern.setAll(false);
            oh_pattern.length = 8;
            oh_pattern.addOne();
            oh_pattern.addOne();
            oh_pattern.abPattern.setConst(0);
            break;
        }

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
                this->hh_pattern.patterns[i].setGate(step, Utils::gate(four_pat, step));
            }
            this->hh_pattern.length = 4;
            this->hh_pattern.abPattern.randomize();
        }
        switch (Rand::distribution(32, 32))
        {
        case 0: this->hat_closed_style = HatClosedStyle::HatClosedRegular; break;
        case 1: this->hat_closed_style = HatClosedStyle::HatClosedInterval; break;
        }

        this->hat_int_pattern.randomizeIntervalHat();
        this->hat_velocity.randomize();
    }

    bool Hats::play_hats_closed()
    {
        switch (this->hat_closed_style)
        {
        case HatClosedStyle::HatClosedInterval:
        {
            TimeDivision div = this->hat_int_pattern.interval(time);
            if (Utils::intervalHit(div, time))
            {
                uint8_t shuffle_delay = 0;
                if (div > TimeDivision::Thirtysecond)
                {
                    shuffle_delay = time.getShuffleDelay(this->timing);
                }
                this->midi_channel->noteOn(
                    NoteStruct(pitch_closed, getVelocity()),
                    shuffle_delay
                );
                return true;
            }
            break;
        }
        case HatClosedStyle::HatClosedRegular:
            if (this->hh_pattern.gate(time))
            {
                this->midi_channel->noteOn(
                    NoteStruct(pitch_closed, getVelocity()),
                    time.getShuffleDelay(this->timing)
                );
                return true;
            }
            break;
        }
        return false;
    }

    bool Hats::play_hats_open()
    {
        if (this->isKilled())
        {
            return false;
        }

        if (this->oh_pattern.gate(time))
        {
            this->midi_channel->noteOn(
                NoteStruct(pitch_open, getVelocity()),
                time.getShuffleDelay(this->timing)
            );
            return true;
        }
        return false;
    }

    bool Hats::play()
    {
        if (this->isKilled())
        {
            return false;
        }

        bool open_played = play_hats_open();
        if (!open_played || !settings.choke_open)
        {
            return play_hats_closed();
        }
    }

    uint8_t Hats::getVelocity()
    {
        uint8_t velocity = 96;
        {
            velocity = Utils::rerange(
                this->hat_velocity.value(time), 
                settings.velocity_range, 
                settings.velocity_offset
            );
        }


        if ((time.tick / TICKS_PER_STEP) % 4 != 2)
        {
            velocity = (uint8_t)(velocity * .8);
        }

        return velocity;
    }
}