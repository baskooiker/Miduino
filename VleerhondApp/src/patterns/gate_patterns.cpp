#include "patterns/gate_patterns.h"

namespace Vleerhond
{
    /////////////////////////////////
    GatePatternAB::GatePatternAB()
    {
        patterns[0].pattern = 0x00;
        patterns[1].pattern = 0x00;
        patterns[2].pattern = 0x00;
        length = 64;
        time_division = TimeDivision::Sixteenth;
    }

    bool GatePatternAB::gate(const TimeStruct& time) const
    {
        if (!Utils::interval_hit(this->time_division, time)) return false;

        uint8_t pat_length = MIN(this->length, 16);
        uint32_t count = time.get_count(this->time_division) % (this->length <= 16 ? pat_length * 4 : 64);
        return this->patterns[this->abPattern.value(count / pat_length)].gate(count % pat_length);
    }

    void GatePatternAB::set_all(bool _value)
    {
        uint8_t value = _value ? 0xFF : 0x00;
        for (int i = 0; i < 3; i++)
            this->patterns[i].pattern = value;
    }

    void GatePatternAB::set(const uint8_t index, bool _value)
    {
        for (int i = 0; i < 3; i++)
        {
            this->patterns[i].set_gate(index, _value);
        }
    }

    void GatePatternAB::set_gates_low()
    {
        for (int i = 0; i < 3; i++)
        {
            this->patterns[i].set_gates_low();
        }
        this->abPattern.set_ab_pattern();
    }

    void GatePatternAB::randomize(const float prob)
    {
        for (int i = 0; i < 3; i++)
            this->patterns[i].randomize(prob);
        this->abPattern.set_ab_pattern();
    }

    void GatePatternAB::randomize_mask_pattern()
    {
        for (int i = 0; i < 3; i++)
        {
            uint8_t from = Rand::randui8(4, 7);
            for (int step = 0; step < 8; step++)
            {
                this->patterns[i].set_gate(step, step > from);
            }
        }

        uint8_t r = Rand::randui8(3);
        if (r < 1)
        {
            this->time_division = TimeDivision::Quarter;
        }
        else if (r < 2)
        {
            this->time_division = TimeDivision::Eighth;
        }
        else
        {
            this->time_division = TimeDivision::Sixteenth;
        }
        this->length = 8;
        this->abPattern.set_ab_pattern();
    }

    void GatePatternAB::set_euclid(const uint8_t length, const uint8_t steps)
    {
        for (int i = 0; i < 3; i++)
        {
            this->patterns[i].set_euclid(length, steps);
        }
        this->abPattern.set_ab_pattern();
    }

    void GatePatternAB::set_diddles(const float f, const bool starts_with, const uint8_t length)
    {
        for (int i = 0; i < 3; i++)
        {
            this->patterns[i].set_diddles(f, starts_with, length);
        }
        this->abPattern.set_ab_pattern();
    }

    void GatePatternAB::set_coef_pattern(const Coefficients coef)
    {
        for (int i = 0; i < 3; i++)
            this->patterns[i].set_coef_pattern(coef);
        this->abPattern.set_ab_pattern();
    }

    void GatePatternAB::set_coef_kick_pattern()
    {
        Coefficients coef = { 0 };
        coef.one = 1.f;
        coef.two = 1.f;
        coef.three = 1.f;
        coef.four = 1.f;
        coef.eights = Rand::randf(.25);
        coef.up = Rand::randf(.125);
        coef.down = Rand::randf(.125);
        set_coef_pattern(coef);
    }

    void GatePatternAB::set_coef_snare_pattern()
    {
        Coefficients coef = { 0 };
        coef.two = Rand::randf(.5f, .75f);
        coef.four = Rand::randf(.5f, .75f);
        coef.eights = Rand::randf(.125);
        coef.up = Rand::randf(.125);
        coef.down = Rand::randf(.125);
        set_coef_pattern(coef);
    }

    void GatePatternAB::set_coef_hat_pattern()
    {
        Coefficients coef = { 0 };
        coef.eights = 1.f;
        coef.up = Rand::randf(.125);
        coef.down = Rand::randf(.125);
        set_coef_pattern(coef);
    }

    void GatePatternAB::set_coef_slow_pattern()
    {
        Coefficients coef = { 0 };
        coef.one = 1.f;
        coef.two = Rand::randf(.5f, 1.f);
        coef.three = Rand::randf(.5f, 1.f);
        coef.four = Rand::randf(.5f, 1.f);
        this->set_coef_pattern(coef);
    }

    void GatePatternAB::remove_one()
    {
        for (int i = 0; i < 3; i++)
        {
            patterns[i].remove_one();
        }
    }

    std::string GatePatternAB::toString()
    {
        std::stringstream ss;
        for (int bar = 0; bar < 4; bar++)
        {
            ss << this->patterns[this->abPattern.value(bar)].toString() << std::endl;
        }
        return ss.str();
    }

}