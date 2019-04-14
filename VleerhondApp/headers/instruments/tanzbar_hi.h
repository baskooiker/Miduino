#pragma once

class TanzbarHi : public InstrumentBase
{
protected:
    TanzbarModulators& tanzbar_modulators;
    TanzbarTimeSettings& tanzbar_time;

public:
    GatePatternAB cy_pattern;
    CvPatternAB ma_pattern;

    TanzbarHi(
        TanzbarModulators& tanzbar_modulators_ref,
        TanzbarTimeSettings& tanzbar_time_ref,
        TimeStruct& time_ref) :
        InstrumentBase(time_ref, true),
        tanzbar_modulators(tanzbar_modulators_ref),
        tanzbar_time(tanzbar_time_ref)
    {
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

    void play()
    {
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
