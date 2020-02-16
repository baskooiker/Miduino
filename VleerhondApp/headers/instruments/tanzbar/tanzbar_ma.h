#pragma once

#include "instruments/instrument_base.h"
#include "midi/midi_io.h"

namespace Vleerhond
{
    enum class PatternType
    {
        Interval,
        Gate
    };

    class RhythmPattern
    {
    public:
        GatePatternAB gate_pattern;
        IntervalPattern interval_pattern;
        PatternType type;

        bool value(const TimeStruct& time)
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

        void randomize_type()
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
    };

    class TanzbarMa : public InstrumentBase
    {
    protected:
        ModulationReceiver decay_mod;

    public:
        CvPatternAB ma_pattern;
        IntervalPattern interval_pattern;

        TanzbarMa(
            Modulators& modulators_ref,
            TimeStruct& time_ref) :
            InstrumentBase(time_ref, true, MIDI_CHANNEL_TANZBAR),
            decay_mod(modulators_ref)
        {
        }

        void randomize()
        {
            ofLogNotice("tanzbar_hi", "randomize()");
            last_randomized_time = Utils::millis();

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
            IntervalProbs interval_probs(0, Rand::randui8(16), 32); 
            interval_pattern.randomize_interval(interval_probs);
            interval_pattern.length = pow(Rand::randui8(3, 5), 2);

            // Modulators
            this->decay_mod.randomize(Rand::randui8(128, 64), 0);

            // Timing
            this->timing.randomize();
        }

        bool play()
        {
            if (isKilled())
                return false;

            if (interval_pattern.hit(time))
            {
                uint8_t value = 64;
                decay_mod.value(time, value);
                MidiIO::send_cc(
                    TB_MA_Decay,
                    Utils::quad(value) / 2,
                    MIDI_CC_CHANNEL_TANZBAR
                );

                this->midi_channel.note_on(
                    NoteStruct(NOTE_TANZBAR_MA, Utils::rerange(this->ma_pattern.value(time), 96, 16)),
                    time.get_shuffle_delay(this->timing)
                );
                return true;
            }
            return false;
        }
    };
}