#pragma once

#include "defs.h"
#include "time_struct.h"
#include "utils.h"

namespace Vleerhond 
{
    class SampleAndHold
    {
    protected:
        uint8_t sampled_value;
        TimeDivision interval;
        uint64_t last_randomized;

    public:
        uint8_t prob;

        SampleAndHold(const TimeDivision div=TimeDivision::Sixteenth)
        {
            interval = div;
        }

        uint8_t value(const TimeStruct& time)
        {
            randomize(time);
            return sampled_value;
        }

        bool gate(const TimeStruct& time)
        {
            randomize(time);
            return value(time) < prob;
        }

    protected:
        void randomize(const TimeStruct time)
        {

            if (!time.interval_hit(interval))
            {
                return;
            }
            if (last_randomized >= time.tick)
            {
                return;
            }
            last_randomized = time.tick;

            this->sampled_value = Rand::randui8();

        }
    };

    class NoteRepeat : public SampleAndHold
    {
    protected:
        TimeDivision interval_value;
        NoteStruct repeated_note;

    public:
        NoteRepeat(const TimeDivision div) :
            SampleAndHold(div)
        {
            interval_value = TimeDivision::Sixteenth;
        }

        TimeDivision get_interval(const TimeStruct& time)
        {
            if (time.interval_hit(interval))
            {
                randomize(time);
            }
            return interval_value;
        }

        NoteStruct repeat_note(const TimeStruct time)
        {
            randomize(time);
            if (gate(time))
            {
                return repeated_note;
            }
            else
            {
                if (repeated_note.pitch > 0)
                {
                    repeated_note = NoteStruct();
                }
                return repeated_note;
            }
        }

        void set_repeat_note(const NoteStruct note)
        {
            if (repeated_note.pitch == 0)
            {
                repeated_note = note;
            }
        }

    protected:
        void randomize(const TimeStruct& time)
        {
            if (!time.interval_hit(interval))
            {
                return;
            }
            if (last_randomized >= time.tick)
            {
                return;
            }

            SampleAndHold::randomize(time);

            switch (Rand::distribution(16, 0))
            {
            case 0: this->interval_value = TimeDivision::Thirtysecond; break;
            case 1: this->interval_value = TimeDivision::Sixteenth; break;
            }

            repeated_note = NoteStruct();
        }
    };

}
