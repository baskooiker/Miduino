#include "core/time_struct.h"

#include "ofLog.h"

#include "core/defs.h"
#include "core/timing_structs.h"

namespace Vleerhond
{
    TimeStruct::TimeStruct()
    {
        tick = 0;
        state = PlayState::Stopped;
        last_pulse_time = 0;
        average_pulse_time = 500.f;
        global_shuffle = 0;
    }

    uint32_t TimeStruct::step() const
    {
        return this->tick / TICKS_PER_STEP;
    }

    uint32_t TimeStruct::getShuffleDelay(
        const int8_t shuffle_offset,
        const uint8_t global_delay) const
    {
        uint32_t delay = (uint32_t)((this->average_pulse_time / 40.) * (global_delay / 127.));

        if ((this->tick + TICKS_PER_STEP) % (2 * TICKS_PER_STEP) == 0)
        {
            uint8_t amount = (uint8_t)CLIP((int8_t)this->global_shuffle + shuffle_offset, 0, 127);
            return MAX((uint32_t)((this->average_pulse_time / 8.) * (amount / 127.)), delay);
        }
        else
            return delay;
    }

    uint32_t TimeStruct::getShuffleDelay(const MicroTimingStruct& micro) const
    {
        return this->getShuffleDelay(micro.shuffle_off, micro.delay);
    }

    uint32_t TimeStruct::getCount(const TimeDivision time_division) const
    {
        return this->tick / (uint32_t)time_division;
    }

    void TimeStruct::randomizeShuffle()
    {
        const int8_t rand_range = 32;
        int8_t min_ = MAX((int8_t)this->global_shuffle - rand_range, 0);
        int8_t max_ = MIN((int8_t)this->global_shuffle + rand_range, 128);
        this->global_shuffle = Rand::randui8(min_, max_);
        ofLogNotice("time", "shuffle = %d", this->global_shuffle);
    }

    uint8_t TimeStruct::ticksLeftInBar() const
    {
        uint32_t tick_in_local_bar = this->tick % TICKS_IN_BAR;
        return TICKS_IN_BAR - tick_in_local_bar;
    }

    void TimeStruct::reset()
    {
        this->tick = 0;
        this->state = PlayState::Stopped;
        this->last_pulse_time = 0;
        this->average_pulse_time = 500.f;
    }

    TimeStruct TimeStruct::add(const int32_t offset)
    {
        TimeStruct time_cp = *this;
        time_cp.tick += offset;
        return time_cp;
    }
}
