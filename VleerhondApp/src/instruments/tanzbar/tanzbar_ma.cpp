#include "instruments/tanzbar/tanzbar_ma.h"

namespace Vleerhond
{
    bool RhythmPattern::value(const TimeStruct & time)
    {
        switch (type)
        {
        default:
        case PatternType::Gate:
            return gate_pattern.gate(time);
        case PatternType::Interval:
            return interval_pattern.hit(time);
        }
    }
    void RhythmPattern::randomize_type()
    {
        if (Rand::distribution(32, 32) == 0)
        {
            type = PatternType::Gate;
        }
        else
        {
            type = PatternType::Interval;
        }
    }
    TanzbarMa::TanzbarMa(Modulators & modulators_ref, TimeStruct & time_ref) :
        InstrumentBase(time_ref),
        decay_mod(modulators_ref)
    {
    }
    void TanzbarMa::randomize()
    {
        ofLogNotice("tanzbar_hi", "randomize()");
        InstrumentBase::randomize();

        // Randomize Seq
        this->ma_pattern.randomize();
        this->ma_pattern.time_division = TimeDivision::Sixteenth;
        switch (Rand::distribution(32, 32, 32))
        {
        case 0: this->ma_pattern.length = 2; break;
        case 1: this->ma_pattern.length = 4; break;
        case 2: this->ma_pattern.length = 8; break;
        }

        // Randomize Interval Pattern
        //IntervalProbs interval_probs(0, Rand::randui8(16), 32);
        interval_pattern.randomize_interval(IntervalProbs(0, 0, 32));
        interval_pattern.length = pow(Rand::randui8(3, 5), 2);

        // Modulators
        this->decay_mod.randomize(Rand::randui8(128, 64), 0);

        // Timing
        this->timing.randomize();
    }
    bool TanzbarMa::play()
    {
        if (isKilled())
            return false;

        if (interval_pattern.hit(time))
        {
            uint8_t value = 64;
            decay_mod.value(time, value);
            getChannel()->sendCC(TB_MA_Decay, Utils::quad(value) / 2);

            this->midi_channel->note_on(
                NoteStruct(NOTE_TANZBAR_MA, Utils::rerange(this->ma_pattern.value(time), 96, 16)),
                time.get_shuffle_delay(this->timing)
            );
            return true;
        }
        return false;
    }
}
