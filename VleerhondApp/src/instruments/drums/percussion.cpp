#include "percussion.h"

#include "instrument_base.h"
#include "patterns/gate_patterns.h"
#include "patterns/modulators.h"

namespace Vleerhond
{
    Percussion::Percussion(
        Modulators& modulators_ref,
        TimeStruct& time_ref) :
        InstrumentBase(time_ref),
        velocity_mod(modulators_ref)
    {
    }

    void Percussion::randomize()
    {
        InstrumentBase::randomize();

        switch (Rand::distribution(settings.p_length_8, settings.p_length_16))
        {
        case 0: 
            pattern.length = 8;
            pattern.abPattern.set_ab_pattern();
            break;
        case 1: 
            pattern.length = 16; 
            pattern.abPattern.set_ab_pattern_const();
            break;
        }

        switch (Rand::distribution(settings.p_rand, settings.p_drop))
        {
        case 0:
            this->pattern.randomize(Rand::randf(.5, 1.0));
            break;
        case 1:
            pattern.set_all(false);
            pattern.add_one();
            pattern.add_one();
            break;
        }

        // Modulators
        uint8_t range = Rand::randui8(128, 64);
        this->velocity_mod.randomize(range, 127 - range);

        this->timing.randomize();
    }

    bool Percussion::play()
    {
        if (this->pattern.gate(time))
        {
            this->midi_channel->note_on(
                NoteStruct(pitch, getVelocity()),
                time.get_shuffle_delay(this->timing)
            );
            return true;
        }
        return false;
    }
    uint8_t Percussion::getVelocity()
    {
        uint8_t velocity = 100;
        velocity_mod.value(time, velocity);
        return velocity;
    }
}