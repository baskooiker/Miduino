#pragma once

class Lfo
{
public:
    uint16_t rate;   // in ticks per cycle
    uint16_t offset; // in ticks per cycle

    Lfo()
    {
        this->rate = TICKS_IN_BAR;
        this->offset = 0;
    }

    uint8_t value(const TimeStruct& time)
    {
        uint32_t phase = (time.tick + this->offset) % this->rate;
        uint16_t half_phase = this->rate / 2;
        if (phase < half_phase)
        {
            return (phase * 127) / half_phase;
        }
        else
        {
            return 127 - (((phase - half_phase) * 127) / half_phase);
        }
    }

    void randomize_offset()
    {
        this->offset = randui16(TICKS_IN_BAR);
    }

    void randomize_sync_high()
    {
        switch (distribution(8, 8, 8, 8))
        {
        case 0: this->rate = TICKS_IN_BAR / 8;
        case 1: this->rate = TICKS_IN_BAR / 4;
        case 2: this->rate = TICKS_IN_BAR / 2;
        case 3: this->rate = TICKS_IN_BAR;
        }
        randomize_offset();
    }

    void randomize_sync_low()
    {
        switch (distribution(8, 8, 8))
        {
        case 0: this->rate = TICKS_IN_BAR;
        case 1: this->rate = TICKS_IN_BAR * 2;
        case 2: this->rate = TICKS_IN_BAR * 4;
        }
        randomize_offset();
    }

    void randomize_high()
    {
        this->rate = randui16(TICKS_IN_BAR, TICKS_PER_STEP * 2);
        randomize_offset();
    }

    void randomize_low()
    {
        this->rate = randui16(TICKS_IN_BAR * 4, TICKS_IN_BAR);
        randomize_offset();
    }

    void randomize()
    {
        switch (distribution(16, 16, 16, 16))
        {
        case 0: this->randomize_low(); break;
        case 1: this->randomize_high(); break;
        case 2: this->randomize_sync_low(); break;
        case 3: this->randomize_sync_high(); break;
        }
    }

};

class Modulators
{
public:
    CvPattern16 pattern_a;
    CvPattern16 pattern_b;

    Lfo lfo_a;
    Lfo lfo_b;
    Lfo lfo_c;
    Lfo lfo_d;

    void randomize()
    {
        lfo_a.randomize();
        lfo_b.randomize();
        lfo_c.randomize();
        lfo_d.randomize();

        this->pattern_a.randomize();
        this->pattern_b.randomize();
    }
};
