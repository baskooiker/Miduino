#pragma once

#include "chords.h"
#include "defs.h"
#include "rand.h"

namespace Vleerhond
{
    class ArpData
    {
    protected:
        uint8_t arp_notes[32];
        uint8_t arp_notes_length;

        RangeType range_type;
        uint8_t last_note;

    public:
        uint8_t min;
        uint8_t range;
        uint8_t range_count;
        uint8_t counter;
        ArpType type;

        ArpData()
        {
            min = 36;
            range = 12;
            range_count = 3;
            counter = 0;
            type = ArpType::UP;
            range_type = RangeType::Range;
            last_note = 0;

            arp_notes_length = 0;
        }

        uint8_t get_closest(
            const uint8_t* arp_pitches,
            const uint8_t size,
            const bool include_current,
            uint8_t& last_note) const
        {
            last_note = Utils::clip(this->last_note, this->min, this->min + this->range);
            if (Utils::is_in_set(this->last_note, arp_pitches, size) && include_current)
                return this->last_note;
            bool below = false;
            bool above = false;
            uint8_t c = 1;
            while (!below || !above)
            {
                if (this->last_note - c > this->min)
                {
                    if (Utils::is_in_set(this->last_note - c, arp_pitches, size))
                    {
                        last_note -= c;
                        return last_note;
                    }
                }
                else
                {
                    below = true;
                }

                if (last_note + c < this->min + this->range)
                {
                    if (Utils::is_in_set(last_note + c, arp_pitches, size))
                    {
                        last_note += c;
                        return last_note;
                    }
                }
                else
                {
                    above = true;
                }
                c++;
            }
            return 0;
        }

        uint8_t get_arp_pitch(
            uint8_t& arp_counter,
            uint8_t &last_note) const
        {
            if (this->arp_notes_length == 0)
                return 0;

            switch (this->type)
            {
            case ArpType::UP:
                arp_counter = arp_counter % this->arp_notes_length;
                return this->arp_notes[arp_counter++];
            case ArpType::DOWN:
                arp_counter = this->counter % this->arp_notes_length;
                return this->arp_notes[this->arp_notes_length - ++arp_counter];
            case ArpType::UPDOWN:
                arp_counter = arp_counter % (2 * this->arp_notes_length - 2);
                if (this->counter < this->arp_notes_length)
                    return this->arp_notes[arp_counter++];
                else
                    return this->arp_notes[this->arp_notes_length - (arp_counter++ - this->arp_notes_length + 2)];
            case ArpType::PICKING_IN:
                arp_counter = arp_counter % this->arp_notes_length;
                if (this->counter % 2 == 0)
                    return this->arp_notes[arp_counter++ / 2];
                else
                    return this->arp_notes[this->arp_notes_length - ++arp_counter / 2];
            case ArpType::CLOSEST:
                return get_closest(this->arp_notes, this->arp_notes_length, true, last_note);
            case ArpType::CLOSEST_EXC:
                return get_closest(this->arp_notes, this->arp_notes_length, false, last_note);
            case ArpType::RANDOM:
                return this->arp_notes[Rand::randui8(this->arp_notes_length)];
            }
            return 0;
        }

        void get_arp_pitches_by_range(const Scale& scale, const uint8_t chord)
        {
            this->arp_notes_length = 0;
            for (int i = this->min; i < this->min + this->range; i++)
            {
                if (scale.chord_contains(i, chord))
                    this->arp_notes[this->arp_notes_length++] = i;
            }
        }

        void get_arp_pitches_by_count(const Scale& scale, const uint8_t chord)
        {
            int i = this->min;
            this->arp_notes_length = 0;
            while (this->arp_notes_length < this->range_count
                && i < 128)
            {
                if (scale.chord_contains(i, chord))
                    this->arp_notes[this->arp_notes_length++] = i;
                i++;
            }
        }

        uint8_t get_next_arp_pitch(const Scale& scale, const uint8_t chord)
        {
            switch (this->range_type)
            {
            case RangeType::Range: this->get_arp_pitches_by_range(scale, chord); break;
            case RangeType::Count: this->get_arp_pitches_by_count(scale, chord); break;
            }

            return this->get_arp_pitch(this->counter, this->last_note);
        }

        uint8_t get_arp_pitch() const
        {
            uint8_t counter = this->counter;
            uint8_t last_note = this->last_note;
            return get_arp_pitch(counter, last_note);
        }

    };
}