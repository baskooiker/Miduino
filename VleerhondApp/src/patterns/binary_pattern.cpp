#include "patterns/binary_pattern.h"

namespace Vleerhond
{

    const std::vector<std::vector<uint8_t>> pattern_twos = {
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

    /////////////////////////////////
    void BinaryPattern::randomize(const float prob)
    {
        for (uint8_t j = 0; j < 16; j++)
        {
            set_gate(j, Rand::randf() < prob);
        }
    }

    void BinaryPattern::set_gate(const uint8_t index, const bool value)
    {
        if (value)
            this->pattern |= (1 << index);
        else
            this->pattern &= ~(1 << index);
    }

    bool BinaryPattern::gate(const uint32_t step, const uint8_t length) const
    {
        return (this->pattern >> (step % length)) & 0x1;
    }

    void BinaryPattern::set_gates_low()
    {
        this->pattern = 0x00;
        uint8_t pattern_id = Rand::randui8(pattern_twos.size());
        for (int i = 0; i < 2; i++)
        {
            this->set_gate(pattern_twos[pattern_id][i], true);
        }
    }

    void BinaryPattern::set_kick_fill(uint8_t offset)
    {
        static const uint8_t fill_a[] = { 1, 0, 0, 1, 0, 0, 1, 0 };
        static const uint8_t fill_b[] = { 1, 0, 1, 0, 0, 1, 0, 0 };
        //static const uint8_t fill_c[] = { 0, 1, 0, 1, 0, 0, 1, 0 };
        //static const uint8_t fill_d[] = { 0, 1, 0, 0, 1, 0, 1, 0 };

        const uint8_t* fill = 0;
        switch (Rand::distribution(10, 10, 0, 0))
        {
        case 0: fill = fill_a; break;
        case 1: fill = fill_b; break;
            //case 2: fill = fill_c; break;
            //case 3: fill = fill_d; break;
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

    void BinaryPattern::set_euclid(const uint8_t _length, const uint8_t _steps)
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

    std::vector<uint8_t> BinaryPattern::distribute(uint8_t size, uint8_t amount)
    {
        ofLogVerbose("PATTERNS", "distribute(size=%d, amount=%d)", size, amount);
        std::vector<uint8_t> sets;

        for (int i = 0; i < size; i++)
        {
            sets.push_back(1);
            if (amount > 0)
                amount -= 1;
        }

        for (int i = 0; i < amount; i++)
        {
            sets[Rand::randui8(size)] += 1;
        }

        return sets;
    }

    void BinaryPattern::set_diddles(const float f, const bool starts_with, uint8_t length)
    {
        length = length > 16 ? 16 : length;
        int8_t nr_hits = (uint8_t)((f * length) + .5);
        uint8_t max_nr_diddles = nr_hits / 2;
        uint8_t nr_diddles = Rand::randui8(2, max_nr_diddles + 1);
        std::vector<uint8_t> sets = distribute(nr_diddles, nr_hits);
        std::vector<uint8_t> spaces = distribute(nr_diddles, length - nr_hits);

        ofLogVerbose("GatePatterns", "set_diddles(nr_hits=%d, nr_diddles=%d, max_nr=%d)", nr_hits, nr_diddles, max_nr_diddles);

        uint8_t count = 0;
        for (int i = 0; i < nr_diddles; i++)
        {
            for (int j = 0; j < sets[i]; j++)
            {
                this->set_gate(count++, starts_with);
            }
            for (int j = 0; j < spaces[i]; j++)
            {
                this->set_gate(count++, !starts_with);
            }
        }
    }

    void BinaryPattern::set_coef_pattern(const Coefficients coef)
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

    void BinaryPattern::add_one(const uint8_t length)
    {
        std::vector<uint8_t> indices;
        for (int i = 0; i < length; i++)
        {
            if (gate(i))
            {
                indices.push_back(i);
            }
        }

        if (indices.size() == 0)
        {
            set_gate(Rand::randui8(8), true);
            return;
        }
        std::random_shuffle(indices.begin(), indices.end());

        for (const uint8_t i: indices)
        {
            if (!gate(i+1 % length))
            {
                set_gate(i+1 % length, false);
                return;
            }
        }
    }

    void BinaryPattern::remove_one()
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

    void BinaryPattern::shift_one()
    {
        if (Rand::randf() < .5)
        {
            if (!shift_down())
            {
                shift_up();
            }

        }
        else
        {
            if (!shift_up())
            {
                shift_down();
            }
        }
    }

    bool BinaryPattern::shift_down()
    {
        ofLogVerbose("PATTERNS", "shift_down");
        std::vector<uint8_t> shiftable;
        for (int i = 0; i < 16; i++)
        {
            if (gate(i) && !gate(i + 15) && !gate(i + 14))
            {
                shiftable.push_back(i);
            }
        }
        if (shiftable.size() == 0)
        {
            return false;
        }
        uint8_t idx = shiftable[Rand::randui8((uint8_t)shiftable.size())];
        ofLogVerbose("PATTERNS", "shift_down: %d", idx);
        this->set_gate(idx, false);
        this->set_gate(idx + 15, true);
        return true;
    }

    bool BinaryPattern::shift_up()
    {
        ofLogVerbose("PATTERNS", "shift_up");
        std::vector<uint8_t> shiftable;
        for (int i = 0; i < 16; i++)
        {
            if (gate(i) && !gate(i + 1) && !gate(i + 2))
            {
                shiftable.push_back(i);
                ofLogVerbose("PATTERNS", "add_shiftable: %d", i);
            }
        }
        if (shiftable.size() == 0)
        {
            return false;
        }
        uint8_t idx = shiftable[Rand::randui8((uint8_t)shiftable.size())];
        ofLogVerbose("PATTERNS", "shift_up: {}", idx);
        this->set_gate(idx, false);
        this->set_gate(idx + 1, true);
        return true;
    }

    std::string BinaryPattern::toString()
    {
        std::stringstream ss;
        for (int i = 0; i < 16; i++)
        {
            ss << (this->gate(i) == true ? "1" : "0") << " ";
        }
        return ss.str();
    }

}
