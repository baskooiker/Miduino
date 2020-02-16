#include "toms.h"

#include "instrument_base.h"
#include "patterns/gate_patterns.h"
#include "patterns/modulators.h"
#include "parameters.h"
#include "patterns/interval_pattern.h"
#include "utils/rand.h"

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
        last_randomized_time = Utils::millis();

        switch (Rand::distribution(16, 16))
        {
        case 0:
            // Randomize toms
            this->tom_pattern.randomize(1, 127);
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 16; j++)
                {
                    if (Rand::randf() < .2)
                    {
                        this->tom_pattern.patterns[j].set(i, false);
                    }
                }
            }
            break;
        case 1:
        {
            tom_pattern.set_all(0);
            int length = 8;
            if (pitches.size() < 4)
            {
                length = Rand::randf() < .5 ? 8 : 16;
            }
            tom_pattern.abPattern.set_ab_pattern_const();
            for (int i = 0; i < pitches.size(); i++)
            {
                std::vector<uint8_t> opts;
                for (int j = 0; j < length; j++) opts.push_back(j);
                std::random_shuffle(opts.begin(), opts.end());
                for (uint8_t opt: opts)
                {
                    if (tom_pattern.patterns[0].value(opt) == 0&&
                        tom_pattern.patterns[0].value(opt + 1 % length) == 0)
                    {
                        tom_pattern.patterns[0].set(opt, i+1);
                        tom_pattern.patterns[0].set(opt + 1 % length, i+1);
                        break;
                    }
                }
            }
            break;
        }
        }

        // Modulators
        uint8_t range = Rand::randui8(16);
        this->tom_vel.randomize(range, 127 - range);

        this->timing.randomize();
    }

    bool Toms::play()
    {
        if (isKilled())
        {
            return false;
        }

        // Play toms
        uint8_t tom_prob = this->tom_pattern.value(time);
        if (Utils::interval_hit(TimeDivision::Sixteenth, time)
            && tom_prob > 0)
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