#pragma once

class TanzbarLo : public InstrumentBase
{
protected:
    TanzbarModulators& tanzbar_modulators;
    TanzbarTimeSettings& tanzbar_time;

public:
    GatePatternAB bd_pattern;

    TanzbarLo(
        TanzbarModulators& tanzbar_modulators_ref,
        TanzbarTimeSettings& tanzbar_time_ref,
        TimeStruct& time_ref) :
        InstrumentBase(time_ref),
        tanzbar_modulators(tanzbar_modulators_ref),
        tanzbar_time(tanzbar_time_ref)
    {
    }

    void randomize()
    {
        ofLogNotice("tanzbar_lo", "randomize()");
        last_randomized_time = millis();

        Tanzbar::randomize_parameters(tanzbar_low_params, nr_tanzbar_low_params);

        this->bd_pattern.set_coef_kick_pattern();

        if (Rand::distribution(32, 12))
        {
            this->randomize_tanzbar_kick();
        }
    }

    void play()
    {
        bool quarter_hit = Utils::interval_hit(TimeDivision::Quarter, time);
        uint8_t velocity = quarter_hit ? 127 : 63;
        if (this->bd_pattern.gate(time) && !this->kill)
        {
            uint8_t pitch = NOTE_TANZBAR_BD1;
            this->storage.note_on(
                NoteStruct(pitch, velocity),
                time.get_shuffle_delay(this->tanzbar_time.bd)
            );
        }

        if (quarter_hit && !this->kill)
        {
            this->storage.note_on(
                NoteStruct(NOTE_TANZBAR_BD2, velocity),
                time.get_shuffle_delay(this->tanzbar_time.bd)
            );
        }
    }

protected:
    void randomize_tanzbar_kick()
    {
        // Fill in first or second half of bar
        uint8_t half = Rand::distribution(64, 64);

        uint8_t bar = this->bd_pattern.abPattern.value(Rand::randui8(4));
        this->bd_pattern.patterns[bar].set_kick_fill(half * 8);
    }

};
