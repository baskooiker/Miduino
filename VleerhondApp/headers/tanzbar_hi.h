#pragma once

class TanzbarHi : public InstrumentBase
{
protected:
    TanzbarModulators& tanzbar_modulators;
    TanzbarTimeSettings& tanzbar_time;

public:
    GatePatternAB hh_pattern;
    GatePatternAB oh_pattern;
    GatePatternAB cy_pattern;
    CvPatternAB ma_pattern;

    IntervalPattern hat_int_pattern;
    CvPatternAB hat_velocity;

    HatClosedStyle hat_closed_style;

    TanzbarHi(
        TanzbarModulators& tanzbar_modulators_ref,
        TanzbarTimeSettings& tanzbar_time_ref,
        TimeStruct& time_ref) :
        InstrumentBase(time_ref),
        tanzbar_modulators(tanzbar_modulators_ref),
        tanzbar_time(tanzbar_time_ref)
    {
        hat_closed_style = HatClosedStyle::HatClosedRegular;
    }

    void randomize()
    {
        ofLogNotice("tanzbar_hi", "randomize()");
        last_randomized_time = millis();

        Tanzbar::randomize_parameters(tanzbar_hi_params, nr_tanzbar_hi_params);

        randomize_hi_seq();
    }

    void randomize_hi_seq()
    {
        // Randomize hats
        this->oh_pattern.set_coef_hat_pattern();
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
                this->hh_pattern.patterns[i].set_gate(step, Utils::gate(four_pat, step));
            }
            this->hh_pattern.length = 4;
            this->hh_pattern.abPattern.set_ab_pattern();
        }
        switch (Rand::distribution(32, 32))
        {
        case 0: this->hat_closed_style = HatClosedStyle::HatClosedRegular; break;
        case 1: this->hat_closed_style = HatClosedStyle::HatClosedInterval; break;
        }

        this->hat_int_pattern.randomize_interval_hat();
        this->hat_velocity.randomize();

        // Randomize Maracas
        this->ma_pattern.randomize();
        this->ma_pattern.time_division = TimeDivision::Sixteenth;
        switch (Rand::distribution(32, 32, 32))
        {
        case 0: this->ma_pattern.length = 2; break;
        case 1: this->ma_pattern.length = 4; break;
        case 2: this->ma_pattern.length = 8; break;
        }

        // Randomize Cymbal
        switch (Rand::distribution(16, 16, 16))
        {
        case 0:
            this->cy_pattern.set_coef_kick_pattern();
            this->cy_pattern.length = 16;
            break;
        case 1:
            this->cy_pattern.set_euclid(8, 3);
            this->cy_pattern.length = 8;
            break;
        case 2:
            this->cy_pattern.set_coef_hat_pattern();
            this->cy_pattern.length = 16;
            break;
        }
    }

    bool play_maracas()
    {
        if (kill)
            return false;

        if (Utils::interval_hit(this->ma_pattern.time_division, time))
        {
            uint8_t value = 64;
            this->tanzbar_modulators.ma_dec.value(time, value);
            MidiIO::send_cc(
                TB_MA_Decay,
                Utils::quad(value) / 2,
                MIDI_CC_CHANNEL_TANZBAR
            );

            this->storage.note_on(
                NoteStruct(NOTE_TANZBAR_MA, Utils::rerange(this->ma_pattern.value(time), 96, 16)),
                time.get_shuffle_delay(this->tanzbar_time.ma)
            );
            return true;
        }
        return false;
    }

    void play_hats_closed()
    {
        uint8_t velocity = 63;

        switch (this->hat_closed_style)
        {
        case HatClosedStyle::HatClosedInterval:
        {
            this->tanzbar_modulators.hats_vel.value(time, velocity);

            TimeDivision div = this->hat_int_pattern.interval(time);
            if (Utils::interval_hit(div, time))
            {
                uint8_t shuffle_delay = 0;
                if (div < TimeDivision::Sixteenth)
                {
                    shuffle_delay = time.get_shuffle_delay(this->tanzbar_time.hh);
                }
                this->storage.note_on(
                    NoteStruct(NOTE_TANZBAR_HH, velocity),
                    shuffle_delay
                );
            }
            break;
        }
        case HatClosedStyle::HatClosedRegular:
            if (this->hh_pattern.gate(time))
            {
                velocity = Utils::rerange(this->hat_velocity.value(time), 50, 32);
                this->storage.note_on(
                    NoteStruct(NOTE_TANZBAR_HH, velocity),
                    time.get_shuffle_delay(this->tanzbar_time.hh)
                );
            }
            break;
        }
    }

    bool play_hats_open()
    {
        if (this->kill)
            return false;

        uint8_t velocity = 63;
        if ((time.tick / TICKS_PER_STEP) % 4 == 2)
            velocity = 127;

        if (this->oh_pattern.gate(time))
        {
            this->storage.note_on(
                NoteStruct(NOTE_TANZBAR_OH, velocity),
                time.get_shuffle_delay(this->tanzbar_time.hh)
            );
            return true;
        }
        return false;
    }

    void play_hats()
    {
        if (this->kill)
            return;

        if (!play_hats_open())
        {
            play_hats_closed();
        }
    }

    void play()
    {
        play_hats();
        play_maracas();

        // Play Cymbal
        if (this->cy_pattern.gate(time) && !kill)
        {
            uint8_t value = 0;
            if (this->tanzbar_modulators.cy_tune.value(time, value))
            {
                MidiIO::send_cc(TB_CY_TUNE, value, MIDI_CC_CHANNEL_TANZBAR);
            }
            uint8_t velocity = 100;
            this->tanzbar_modulators.cy_vel.value(time, velocity);
            this->storage.note_on(
                NoteStruct(NOTE_TANZBAR_CY, 64),
                time.get_shuffle_delay(this->tanzbar_time.cy)
            );
        }
    }
};
