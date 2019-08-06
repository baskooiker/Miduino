#pragma once

#include "ab.h"
#include "defs.h"
#include "rand.h"
#include "time_struct.h"
#include "intervals.h"

#define NR_OF_TWO_PATTERNS 14

namespace Vleerhond
{
    const uint8_t pattern_twos[NR_OF_TWO_PATTERNS][2] = {
        {0,  6},
        {0,  7},
        {0, 10},
        {0, 11},
        {0, 12},
        {0, 12},
        {0, 15},
        {2,  6},
        {2,  7},
        {2, 10},
        {2, 11},
        {2, 12},
        {2, 12},
        {2, 15},
    };

    class BinaryPattern
    {
    public:
        uint16_t pattern;

        void randomize(const float prob)
        {
            for (uint8_t j = 0; j < 16; j++)
            {
                set_gate(j, Rand::randf() < prob);
            }
        }

        void set_gate(const uint8_t index, const bool value)
        {
            if (value)
                this->pattern |= (1 << index);
            else
                this->pattern &= ~(1 << index);
        }

        bool gate(const uint32_t step, const uint8_t length = 16) const
        {
            return (this->pattern >> (step % length)) & 0x1;
        }

        void set_gates_low()
        {
            this->pattern = 0x00;
            uint8_t pattern_id = Rand::randui8(NR_OF_TWO_PATTERNS);
            for (int i = 0; i < 2; i++)
            {
                this->set_gate(pattern_twos[pattern_id][i], true);
            }
        }

        void set_kick_fill(uint8_t offset)
        {
            static const uint8_t fill_a[] = { 1, 0, 0, 1, 0, 0, 1, 0 };
            static const uint8_t fill_b[] = { 1, 0, 1, 0, 0, 1, 0, 0 };
            static const uint8_t fill_c[] = { 0, 1, 0, 1, 0, 0, 1, 0 };
            static const uint8_t fill_d[] = { 0, 1, 0, 0, 1, 0, 1, 0 };

            const uint8_t* fill = 0;
            switch (Rand::distribution(10, 10, 10, 10))
            {
            case 0: fill = fill_a; break;
            case 1: fill = fill_b; break;
            case 2: fill = fill_c; break;
            case 3: fill = fill_d; break;
            }

            for (int i = 0; i < 8; i++)
            {
                uint8_t index = offset + i;
                if (index < 16)
                {
                    this->set_gate(index, fill[i]);
                }
            }
        }

        void set_euclid(const uint8_t _length, const uint8_t _steps)
        {
            uint8_t length = MIN(_length, 16);
            uint8_t steps = MIN(_steps, length);

            this->pattern = 0x00;

            uint8_t counters[16] = { 0 };
            for (int i = 0; i < length; i++)
                counters[i%steps]++;

            Utils::randomize_order(counters, steps);

            uint8_t c = 0;
            for (int i = 0; i < steps; i++)
            {
                this->set_gate(c, true);
                c += counters[i];
            }
        }

        void set_coef_pattern(const Coefficients coef)
        {
            this->set_gate(0, Rand::randf() < coef.one);
            this->set_gate(4, Rand::randf() < coef.two);
            this->set_gate(8, Rand::randf() < coef.three);
            this->set_gate(12, Rand::randf() < coef.four);
            for (int i = 2; i < 16; i += 4)
                this->set_gate(i, Rand::randf() < coef.eights);
            for (int i = 1; i < 16; i += 4)
                this->set_gate(i, Rand::randf() < coef.down);
            for (int i = 3; i < 16; i += 4)
                this->set_gate(i, Rand::randf() < coef.up);
        }

        void remove_one()
        {
            uint8_t indices[16];
            for (int i = 0; i < 16; i++)
            {
                indices[i] = i;
            }
            Utils::randomize_order(indices, 16);
            for (int i = 0; i < 16; i++)
            {
                if (gate(i))
                {
                    set_gate(i, false);
                    return;
                }
            }
        }
    };

    class GatePattern16
    {
    public:
        BinaryPattern pattern;
        uint8_t length;

        GatePattern16()
        {
            pattern.pattern = 0x00;
            length = 16;
        }

        bool gate(const TimeStruct& time) const
        {
            if (time.tick % TICKS_PER_STEP != 0)
                return false;
            return this->pattern.gate((time.tick / TICKS_PER_STEP) % this->length);
        }

        void randomize(const float prob)
        {
            this->pattern.randomize(prob);
        }

    };

    class GatePatternAB
    {
    public:
        BinaryPattern patterns[3];
        AbPattern abPattern;
        TimeDivision time_division;
        uint8_t length;

        GatePatternAB()
        {
            patterns[0].pattern = 0x00;
            patterns[1].pattern = 0x00;
            patterns[2].pattern = 0x00;
            length = 64;
            time_division = TimeDivision::Sixteenth;
        }

        bool gate(const TimeStruct& time) const
        {
            if (!time.interval_hit(this->time_division)) return false;

            uint8_t pat_length = MIN(this->length, 16);
            uint32_t count = time.get_count(this->time_division) % (this->length <= 16 ? pat_length * 4 : 64);
            return this->patterns[this->abPattern.value(count / pat_length)].gate(count % pat_length);
        }

        void set_all(bool _value)
        {
            uint8_t value = _value ? 0xFF : 0x00;
            for (int i = 0; i < 3; i++)
                this->patterns[i].pattern = value;
        }

        void set(const uint8_t index, bool _value)
        {
            for (int i = 0; i < 3; i++)
            {
                this->patterns[i].set_gate(index, _value);
            }
        }

        void set_gates_low()
        {
            for (int i = 0; i < 3; i++)
            {
                this->patterns[i].set_gates_low();
            }
            this->abPattern.set_ab_pattern();
        }

        void randomize(float prob = .5f)
        {
            for (int i = 0; i < 3; i++)
                this->patterns[i].randomize(prob);
            this->abPattern.set_ab_pattern();
        }

        void randomize_mask_pattern()
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

        void set_euclid(const uint8_t length, const uint8_t steps)
        {
            for (int i = 0; i < 3; i++)
            {
                this->patterns[i].set_euclid(length, steps);
            }
            this->abPattern.set_ab_pattern();
        }

        void set_coef_pattern(const Coefficients coef)
        {
            for (int i = 0; i < 3; i++)
                this->patterns[i].set_coef_pattern(coef);
            this->abPattern.set_ab_pattern();
        }

        void set_coef_kick_pattern()
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

        void set_coef_snare_pattern()
        {
            Coefficients coef = { 0 };
            coef.two = Rand::randf(.5f, .75f);
            coef.four = Rand::randf(.5f, .75f);
            coef.eights = Rand::randf(.125);
            coef.up = Rand::randf(.125);
            coef.down = Rand::randf(.125);
            set_coef_pattern(coef);
        }

        void set_coef_hat_pattern()
        {
            Coefficients coef = { 0 };
            coef.eights = 1.f;
            coef.up = Rand::randf(.25);
            coef.down = Rand::randf(.25);
            set_coef_pattern(coef);
        }

        void set_coef_slow_pattern()
        {
            Coefficients coef = { 0 };
            coef.one = 1.f;
            coef.two = Rand::randf(.5f, 1.f);
            coef.three = Rand::randf(.5f, 1.f);
            coef.four = Rand::randf(.5f, 1.f);
            this->set_coef_pattern(coef);
        }

        void remove_one()
        {
            for (int i = 0; i < 3; i++)
            {
                patterns[i].remove_one();
            }
        }
    };
}