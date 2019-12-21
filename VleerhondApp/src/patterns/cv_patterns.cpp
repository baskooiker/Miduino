#include "patterns/cv_patterns.h"

namespace Vleerhond
{
    void CvPattern::randomize(const uint8_t maximum, const uint8_t minimum)
    {
        for (int i = 0; i < STEPS_IN_BAR; i++)
        {
            this->pattern[i] = Rand::randui8(minimum, maximum);
        }
    }

    void CvPattern::set(const uint8_t i, const uint8_t value)
    {
        this->pattern[i] = value;
    }

    void CvPattern::set_all(const uint8_t value)
    {
        for (int i = 0; i < STEPS_IN_BAR; i++)
            this->pattern[i] = value;
    }

    uint8_t CvPattern::value(const uint8_t step) const
    {
        return this->pattern[step % STEPS_IN_BAR];
    }

    std::string CvPattern::toString(const int length)
    {
        std::stringstream ss;
        for (int i = 0; i < length; i++)
        {
            ss << std::to_string(this->value(i)) << " ";
        }
        return ss.str();
    }

    ///////////////////////////////////////////////////////////////////

    CvPattern16::CvPattern16()
    {
        this->length = STEPS_IN_BAR;
        this->time_division = TimeDivision::Sixteenth;
    }

    void CvPattern16::randomize(const uint8_t maximum, const uint8_t minimum)
    {
        this->pattern.randomize(maximum, minimum);
    }

    uint8_t CvPattern16::value(const TimeStruct& time) const
    {
        return this->value(time.get_count(this->time_division));
    }

    uint8_t CvPattern16::value(const uint8_t step) const
    {
        return this->pattern.value(step % MAX(MIN(this->length, 16), 1));
    }

    void CvPattern16::set_all(const uint8_t value)
    {
        this->pattern.set_all(value);
    }

    void CvPattern16::set(const uint8_t index, const uint8_t value)
    {
        this->pattern.set(index, value);
    }

    // TODO: Move to separate ChordPattern class
    void CvPattern16::set_chord_pattern(
        const Scale& scale,
        const uint8_t start_chord)
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

    //////////////////////////////////////////////////////////////////////////////////

    CvPatternAB::CvPatternAB()
    {
        length = 64;
        time_division = TimeDivision::Sixteenth;
    }

    uint8_t CvPatternAB::value(const TimeStruct& time) const
    {
        uint8_t pat_length = MIN(this->length, 16);
        uint32_t count = time.get_count(this->time_division) % (this->length <= 16 ? pat_length * 4 : 64);
        return this->patterns[this->abPattern.value(count / pat_length)].value(count % pat_length);
    }

    void CvPatternAB::set_all(const uint8_t value)
    {
        for (int i = 0; i < 3; i++)
            this->patterns[i].set_all(value);
    }

    void CvPatternAB::randomize(const uint8_t max, const uint8_t min)
    {
        for (int i = 0; i < 3; i++)
        {
            this->patterns[i].randomize(max, min);
        }
        this->abPattern.set_ab_pattern();
    }

    std::string CvPatternAB::toString()
    {
        std::stringstream ss;
        for (int i = 0; i < 4; i++)
        {
            ss << this->patterns[this->abPattern.value(i)].toString(this->length) << "\n";
        }
        return ss.str();
    }
}
