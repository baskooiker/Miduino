#include "patterns/modulators.h"

namespace Vleerhond
{
    Lfo::Lfo()
    {
        this->rate = TICKS_IN_BAR;
        this->offset = 0;
    }

    uint8_t Lfo::value(const TimeStruct& time) const
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

    void Lfo::randomize_offset()
    {
        this->offset = Rand::randui16(TICKS_IN_BAR);
    }

    void Lfo::randomize_sync_high()
    {
        switch (Rand::distribution(8, 8, 8, 8))
        {
        case 0: this->rate = TICKS_IN_BAR / 8;
        case 1: this->rate = TICKS_IN_BAR / 4;
        case 2: this->rate = TICKS_IN_BAR / 2;
        case 3: this->rate = TICKS_IN_BAR;
        }
        randomize_offset();
    }

    void Lfo::randomize_sync_low()
    {
        switch (Rand::distribution(8, 8, 8))
        {
        case 0: this->rate = TICKS_IN_BAR;
        case 1: this->rate = TICKS_IN_BAR * 2;
        case 2: this->rate = TICKS_IN_BAR * 4;
        }
        randomize_offset();
    }

    void Lfo::randomize_high()
    {
        this->rate = Rand::randui16(TICKS_IN_BAR, TICKS_PER_STEP * 2);
        randomize_offset();
    }

    void Lfo::randomize_low()
    {
        this->rate = Rand::randui16(TICKS_IN_BAR * 4, TICKS_IN_BAR);
        randomize_offset();
    }

    void Lfo::randomize()
    {
        ofLogVerbose("modulators", "randomize()");
        switch (Rand::distribution(16, 16, 16, 16))
        {
        case 0: this->randomize_low(); break;
        case 1: this->randomize_high(); break;
        case 2: this->randomize_sync_low(); break;
        case 3: this->randomize_sync_high(); break;
        }
    }


    void Modulators::randomize()
    {
        ofLogNotice("modulators", "randomize()");
        lfo_a.randomize();
        lfo_b.randomize();
        lfo_c.randomize();
        lfo_d.randomize();

        this->pattern_a.randomize();
        this->pattern_b.randomize();
    }

    ModulationReceiver::ModulationReceiver(Modulators& modulators) :
        modulators(modulators)
    {
        minimum = 0;
        range = 127;
        inverse = false;
        type = ModulationType::ModLfoA;
    }

    bool ModulationReceiver::value(
        const TimeStruct& time,
        uint8_t& value) const
    {
        switch (type)
        {
        case ModulationType::ModLfoA:
            value = modulators.lfo_a.value(time);
            break;
        case ModulationType::ModLfoB:
            value = modulators.lfo_b.value(time);
            break;
        case ModulationType::ModLfoC:
            value = modulators.lfo_c.value(time);
            break;
        case ModulationType::ModLfoD:
            value = modulators.lfo_d.value(time);
            break;
        case ModulationType::ModPatA:
            value = modulators.pattern_a.value(time);
            break;
        case ModulationType::ModPatB:
            value = modulators.pattern_b.value(time);
            break;
        case ModulationType::ModRandom:
            value = Rand::randui8();
            break;
        case ModulationType::ModOff:
            return false;
        }

        if (this->inverse)
        {
            value = 127 - value;
        }

        value = Utils::rerange(value, this->range, this->minimum);

        return true;
    }

    uint8_t ModulationReceiver::value(const Modulators& modulators, const TimeStruct& time)
    {
        uint8_t value = 0;
        this->value(time, value);
        return value;
    }

    void ModulationReceiver::randomize()
    {
        ofLogNotice("modulators", "randomize()");
        switch (Rand::randui8(7))
        {
        default:
        case 0:  this->type = ModLfoA;    break;
        case 1:  this->type = ModLfoB;    break;
        case 2:  this->type = ModLfoC;    break;
        case 3:  this->type = ModLfoD;    break;
        case 4:  this->type = ModPatA;    break;
        case 5:  this->type = ModPatB;    break;
        case 6: this->type = ModRandom;  break;
        }
        this->inverse = Rand::randf() < .5;
    }

    void ModulationReceiver::randomize(const uint8_t range, const uint8_t offset, const float prob)
    {
        if (Rand::randf() < prob)
        {
            this->randomize();
            this->range = range;
            this->minimum = offset;
        }
        else
        {
            this->type = ModulationType::ModOff;
        }
    }

}