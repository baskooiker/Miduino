#pragma once

class TanzbarMid : public InstrumentBase
{
protected:
    TanzbarModulators& tanzbar_modulators;
    TanzbarTimeSettings& tanzbar_time;

    GatePatternAB sd_pattern;
    GatePatternAB rs_pattern;
    GatePatternAB cp_pattern;

public:
    uint8_t snare_roll;

    TanzbarMid(
        TanzbarModulators& tanzbar_modulators_ref,
        TanzbarTimeSettings& tanzbar_time_ref,
        TimeStruct& time_ref) :
        InstrumentBase(time_ref, true),
        tanzbar_modulators(tanzbar_modulators_ref),
        tanzbar_time(tanzbar_time_ref)
    {
        storage.set_channel(MIDI_CHANNEL_TANZBAR);
    }

    void randomize()
    {
        ofLogNotice("tanzbar_mid", "randomize()");
        last_randomized_time = millis();

        Tanzbar::randomize_parameters(tanzbar_mid_params, nr_tanzbar_mid_params);

        this->sd_pattern.set_coef_snare_pattern();
        this->cp_pattern.set_coef_snare_pattern();
        this->rs_pattern.randomize();
    }

    void play_roll(const TimeStruct& time)
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
            this->storage.note_on(
                NoteStruct(NOTE_TANZBAR_SD, this->snare_roll),
                time.get_shuffle_delay()
            );
        }
    }

    void play()
    {
        if (this->snare_roll > 0)
        {
            this->play_roll(time);
        }

        // Play snare
        if (this->sd_pattern.gate(time) && !this->kill)
        {
            this->storage.note_on(
                NoteStruct(NOTE_TANZBAR_SD, 64),
                time.get_shuffle_delay(this->tanzbar_time.sd)
            );
        }

        // Play rimshot
        if (this->rs_pattern.gate(time) && !this->kill)
        {
            uint8_t value = 0;
            if (this->tanzbar_modulators.rs_tune.value(time, value))
            {
                MidiIO::send_cc(TB_RS_TUNE, value, MIDI_CC_CHANNEL_TANZBAR);
            }
            this->storage.note_on(
                NoteStruct(NOTE_TANZBAR_RS, 64),
                time.get_shuffle_delay(this->tanzbar_time.sd)
            );
        }

        // Play clap
        if (this->cp_pattern.gate(time) && !this->kill)
        {
            uint8_t value = 0;
            if (this->tanzbar_modulators.cp_trig.value(time, value))
            {
                MidiIO::send_cc(TB_CP_TRIGGER, value, MIDI_CC_CHANNEL_TANZBAR);
            }
            this->storage.note_on(
                NoteStruct(NOTE_TANZBAR_CP, 64),
                time.get_shuffle_delay(this->tanzbar_time.cp)
            );
        }
    }
};
