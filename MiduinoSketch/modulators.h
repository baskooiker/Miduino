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

    uint8_t value(const TimeStruct& time) const
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

typedef enum
{
    ModOff,
    ModLfoA,
    ModLfoB,
    ModLfoC,
    ModLfoD,
    ModPatA,
    ModPatB,
    ModLfoAInv,
    ModLfoBInv,
    ModLfoCInv,
    ModLfoDInv,
    ModPatAInv,
    ModPatBInv,
    ModRandom
} ModulationType;

class ModulationReceiver
{
public:
    uint8_t minimum;
    uint8_t range;
    ModulationType type;

    bool value(
        const Modulators& modulators, 
        const TimeStruct& time,
        uint8_t& value) const
    {
        switch (type)
        {
        case ModulationType::ModLfoA:
            value = modulators.lfo_a.value(time);
        case ModulationType::ModLfoAInv:
            value = 127 - value;
            break;
        case ModulationType::ModLfoB:
            value = modulators.lfo_b.value(time);
        case ModulationType::ModLfoBInv:
            value = 127 - value;
            break;
        case ModulationType::ModLfoC:
            value = modulators.lfo_c.value(time);
        case ModulationType::ModLfoCInv:
            value = 127 - value;
            break;
        case ModulationType::ModLfoD:
            value = modulators.lfo_d.value(time);
        case ModulationType::ModLfoDInv:
            value = 127 - value;
            break;
        case ModulationType::ModPatA:
            value = modulators.pattern_a.value(time);
        case ModulationType::ModPatAInv:
            value = 127 - value;
            break;
        case ModulationType::ModPatB:
            value = modulators.pattern_b.value(time);
        case ModulationType::ModPatBInv:
            value = 127 - value;
            break;
        case ModulationType::ModRandom:
            value = randui8();
            break;
        case ModulationType::ModOff:
            return false;
        }

        value = rerange(value, this->range, this->minimum);

        return true;
    }

protected:
    uint8_t value(const Modulators& modulators, const TimeStruct& time)
    {
        uint8_t value = 0;
        this->value(modulators, time, value);
        return value;
    }

public:
    void randomize()
    {
        switch (randui8(13))
        {
        default:
        case 0:  this->type = ModLfoA;    break;
        case 1:  this->type = ModLfoB;    break;
        case 2:  this->type = ModLfoC;    break;
        case 3:  this->type = ModLfoD;    break;
        case 4:  this->type = ModPatA;    break;
        case 5:  this->type = ModPatB;    break;
        case 6:  this->type = ModLfoAInv; break;
        case 7:  this->type = ModLfoBInv; break;
        case 8:  this->type = ModLfoCInv; break;
        case 9:  this->type = ModLfoDInv; break;
        case 10: this->type = ModPatAInv; break;
        case 11: this->type = ModPatBInv; break;
        case 12: this->type = ModRandom;  break;
        }
    }

    void randomize(const uint8_t range, const uint8_t offset, const float prob = 2.f)
    {
        if (randf() < prob)
        {
            this->randomize();
        }
        else
        {
            this->type = ModulationType::ModOff;
        }
        this->range = range;
        this->minimum = offset;
    }

};
