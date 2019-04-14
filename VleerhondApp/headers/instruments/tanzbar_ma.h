#pragma once

class TanzbarMa : public InstrumentBase
{
protected:
    ModulationReceiver ma_dec;
    MicroTimingStruct timing;

public:
    CvPatternAB ma_pattern;

    TanzbarMa(
        Modulators& modulators_ref,
        TimeStruct& time_ref) :
        InstrumentBase(time_ref, true),
        ma_dec(modulators_ref)
    {
        storage.set_channel(MIDI_CHANNEL_TANZBAR);
    }

    void randomize()
    {
        ofLogNotice("tanzbar_hi", "randomize()");
        last_randomized_time = millis();

        // Randomize Seq
        this->ma_pattern.randomize();
        this->ma_pattern.time_division = TimeDivision::Sixteenth;
        switch (Rand::distribution(32, 32, 32))
        {
        case 0: this->ma_pattern.length = 2; break;
        case 1: this->ma_pattern.length = 4; break;
        case 2: this->ma_pattern.length = 8; break;
        }

        // Modulators
        this->ma_dec.randomize(Rand::randui8(128, 64), 0);

        // Timing
        this->timing.randomize();
    }

    void play()
    {
        if (kill)
            return;

        if (Utils::interval_hit(this->ma_pattern.time_division, time))
        {
            uint8_t value = 64;
            ma_dec.value(time, value);
            MidiIO::send_cc(
                TB_MA_Decay,
                Utils::quad(value) / 2,
                MIDI_CC_CHANNEL_TANZBAR
            );

            this->storage.note_on(
                NoteStruct(NOTE_TANZBAR_MA, Utils::rerange(this->ma_pattern.value(time), 96, 16)),
                time.get_shuffle_delay(this->timing)
            );
            return;
        }
        return;
    }
};
