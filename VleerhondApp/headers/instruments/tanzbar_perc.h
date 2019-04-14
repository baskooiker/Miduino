#pragma once

class TanzbarPerc : public InstrumentBase
{
protected:
    TanzbarModulators& tanzbar_modulators;
    TanzbarTimeSettings& tanzbar_time;

    GatePatternAB cl_pattern;
    GatePatternAB cb_pattern;
    CvPatternAB tom_pattern;

    PercussionType percussion_type;
    GatePatternAB tom_mask;

public:

    TanzbarPerc(
        TanzbarModulators& tanzbar_modulators_ref,
        TanzbarTimeSettings& tanzbar_time_ref,
        TimeStruct& time_ref) :
        InstrumentBase(time_ref, true),
        tanzbar_modulators(tanzbar_modulators_ref),
        tanzbar_time(tanzbar_time_ref)
    {
        storage.set_channel(MIDI_CHANNEL_TANZBAR);
        percussion_type = PercussionType::PercussionToms;
    }

    void randomize()
    {
        ofLogNotice("tanzbar_perc", "randomize()");
        last_randomized_time = millis();

        Tanzbar::randomize_parameters(tanzbar_perc_params, nr_tanzbar_perc_params);

        // Randomize toms
        this->tom_pattern.randomize();
        switch (Rand::distribution(32, 32))
        {
        case 0: this->percussion_type = PercussionType::PercussionToms; break;
        case 1: this->percussion_type = PercussionType::PercussionCongas; break;
        }
        this->tom_mask.randomize_mask_pattern();

        // Randomize other percussion
        this->cl_pattern.randomize();
        this->cb_pattern.randomize();
    }

    void play()
    {
        // Play clave
        if (this->cl_pattern.gate(time))
        {
            uint8_t value = 0;
            if (this->tanzbar_modulators.cl_pitch.value(time, value))
            {
                MidiIO::send_cc(TB_CL_TUNE, value, MIDI_CC_CHANNEL_TANZBAR);
            }
            this->storage.note_on(
                NoteStruct(NOTE_TANZBAR_CL, 64),
                time.get_shuffle_delay(this->tanzbar_time.cl)
            );
        }

        // Play cowbell
        if (this->cb_pattern.gate(time) && !kill)
        {
            uint8_t value = 0;
            if (this->tanzbar_modulators.cb_tune.value(time, value))
            {
                MidiIO::send_cc(TB_CB_Tune, value, MIDI_CC_CHANNEL_TANZBAR);
            }
            this->storage.note_on(
                NoteStruct(NOTE_TANZBAR_CB, 64),
                time.get_shuffle_delay(this->tanzbar_time.cb)
            );
        }

        // Play toms
        uint8_t tom_prob = this->tom_pattern.value(time);
        if (Utils::interval_hit(TimeDivision::Sixteenth, time)
            && tom_prob < 100
            && this->tom_mask.gate(time)
            && !kill)
        {
            uint8_t tom_id = tom_prob % 3;
            uint8_t velocity = 100;
            this->tanzbar_modulators.tom_vel.value(time, velocity);

            this->storage.note_on(
                NoteStruct(get_tc_pitch(tom_id, this->percussion_type), velocity),
                time.get_shuffle_delay(this->tanzbar_time.tc)
            );
        }

    }

protected:
    uint8_t get_tc_pitch(const uint8_t id, const PercussionType type)
    {
        static const uint8_t t_pitches[3] = { NOTE_TANZBAR_LT, NOTE_TANZBAR_MT, NOTE_TANZBAR_HT };
        static const uint8_t c_pitches[3] = { NOTE_TANZBAR_LC, NOTE_TANZBAR_MC, NOTE_TANZBAR_HC };
        switch (type)
        {
        case PercussionType::PercussionToms:
            return t_pitches[id % 3];
        case PercussionType::PercussionCongas:
        default:
            return c_pitches[id % 3];
        }
    }

};
