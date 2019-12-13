#include "percussion.h"

#include "instrument_base.h"
#include "gate_patterns.h"
#include "modulators.h"
#include "parameters.h"

namespace Vleerhond
{
    Percussion::Percussion(
        Modulators& modulators_ref,
        TimeStruct& time_ref,
        const uint8_t midi_channel) :
        InstrumentBase(time_ref, true, midi_channel),
        velocity_mod(modulators_ref)
    {
    }

    void Percussion::randomize()
    {
        last_randomized_time = millis();

        switch (Rand::distribution(32, 16))
        {
        case 0:
            this->pattern.randomize(Rand::randf(.5, 1.0));
            switch (Rand::distribution(settings.p_length_8, settings.p_length_16))
            {
            case 0: pattern.length = 8; break;
            case 1: pattern.length = 16; break;
            }
            break;
        case 1:
            this->pattern.set_coef_kick_pattern();
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
            this->midi_channel.note_on(
                NoteStruct(pitch, get_velocity()),
                time.get_shuffle_delay(this->timing)
            );
            return true;
        }
        return false;
    }
    uint8_t Percussion::get_velocity()
    {
        uint8_t velocity = 100;
        velocity_mod.value(time, velocity);
        return velocity;
    }
}