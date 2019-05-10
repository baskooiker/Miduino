#pragma once

#include "ab.h"
#include "consts.h"
#include "enums.h"
#include "rand.h"
#include "time_struct.h"
#include "utils.h"
#include "scale.h"
#include "chords.h"

namespace Vleerhond
{
    class CvPattern
    {
    public:
        uint8_t pattern[STEPS_IN_BAR];

        void randomize(const uint8_t maximum = 128, const uint8_t minimum = 0)
        {
            for (int i = 0; i < STEPS_IN_BAR; i++)
            {
                this->pattern[i] = Rand::randui8(minimum, maximum);
            }
        }

        void set(const uint8_t i, const uint8_t value)
        {
            this->pattern[i] = value;
        }

        void set_all(const uint8_t value)
        {
            for (int i = 0; i < STEPS_IN_BAR; i++)
                this->pattern[i] = value;
        }

        uint8_t value(const uint8_t step) const
        {
            return this->pattern[step % STEPS_IN_BAR];
        }
    };

    class CvPattern16 {
    public:
        CvPattern pattern;
        uint8_t length;
        TimeDivision time_division;

        CvPattern16()
        {
            this->length = STEPS_IN_BAR;
            this->time_division = TimeDivision::Sixteenth;
        }

        void randomize(const uint8_t maximum = 128, const uint8_t minimum = 0)
        {
            this->pattern.randomize(maximum, minimum);
        }

        uint8_t value(const TimeStruct& time) const
        {
            return this->value(time.get_count(this->time_division));
        }

        uint8_t value(const uint8_t step) const
        {
            return this->pattern.value(step % MAX(MIN(this->length, 16), 1));
        }

        void set_all(const uint8_t value)
        {
            this->pattern.set_all(value);
        }

        void set(const uint8_t index, const uint8_t value)
        {
            this->pattern.set(index, value);
        }

        // TODO: Move to separate ChordPattern class
        void set_chord_pattern(
            const Scale& scale,
            const uint8_t start_chord = 0)
        {
            uint8_t options[8] = { 0 };
            uint8_t options_length = 0;
            scale.get_available_chords_indices(options, options_length);
            Utils::randomize_order(options, options_length);

            uint8_t start_chord_idx = 0;
            Utils::find_item(start_chord, options, options_length, start_chord_idx);
            Utils::swap(options, start_chord_idx, 0);

            uint8_t time_pattern_length = 0;
            uint8_t chord_time_pattern[4] = { 0, 0, 0, 0 };
            ChordUtils::get_chord_time_pattern(chord_time_pattern, time_pattern_length);

            uint8_t seq[4] = { 0, 0, 0, 0 };
            ChordUtils::get_chord_seq(options, time_pattern_length, seq);

            uint8_t c = 0;
            for (int i = 0; i < time_pattern_length && i < 4; i++)
            {
                for (int j = 0; j < chord_time_pattern[i]; j++)
                {
                    this->set(c, seq[i]);
                    c += 1;
                }
            }
        }

    };

    class CvPatternAB
    {
    public:
        CvPattern patterns[3];
        AbPattern abPattern;
        TimeDivision time_division;
        uint8_t length;

        CvPatternAB()
        {
            length = 64;
            time_division = TimeDivision::Sixteenth;
        }

        uint8_t value(const TimeStruct& time) const
        {
            uint8_t pat_length = MIN(this->length, 16);
            uint32_t count = time.get_count(this->time_division) % (this->length <= 16 ? pat_length * 4 : 64);
            return this->patterns[this->abPattern.value(count / pat_length)].value(count % pat_length);
        }

        void set_all(const uint8_t value)
        {
            for (int i = 0; i < 3; i++)
                this->patterns[i].set_all(value);
        }

        void randomize(const uint8_t max = 128, const uint8_t min = 0)
        {
            for (int i = 0; i < 3; i++)
            {
                this->patterns[i].randomize(max, min);
            }
            this->abPattern.set_ab_pattern();
        }
    };
}