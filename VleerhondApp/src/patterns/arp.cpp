#include "patterns/arp.h"

#include "core/defs.h"
#include "harmony/chords.h"
#include "utils/rand.h"

namespace Vleerhond
{
    ArpData::ArpData()
    {
        min = 36;
        range = 12;
        range_count = 3;
        counter = 0;
        type = ArpType::UP;
        range_type = RangeType::Range;
        last_note = 0;
    }

    uint8_t ArpData::getClosest(
        const std::vector<uint8_t> arp_pitches,
        const bool include_current,
        uint8_t& last_note) const
    {
        last_note = CLIP(this->last_note, this->min, this->min + this->range);
        if (Utils::isInSet(this->last_note, arp_pitches) && include_current)
            return this->last_note;
        bool below = false;
        bool above = false;
        uint8_t c = 1;
        while (!below || !above)
        {
            if (this->last_note - c > this->min)
            {
                if (Utils::isInSet(this->last_note - c, arp_pitches))
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
                if (Utils::isInSet(last_note + c, arp_pitches))
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

    uint8_t ArpData::get_arp_pitch(
        uint8_t& arp_counter,
        uint8_t &last_note) const
    {
        if (this->arp_notes.size() == 0)
        {
            return 0;
        }

        switch (this->type)
        {
        case ArpType::UP:
            arp_counter = arp_counter % this->arp_notes.size();
            return this->arp_notes[arp_counter++];
        case ArpType::DOWN:
            arp_counter = this->counter % this->arp_notes.size();
            return this->arp_notes[this->arp_notes.size() - ++arp_counter];
        case ArpType::UPDOWN:
            arp_counter = arp_counter % (2 * this->arp_notes.size() - 2);
            if (this->counter < this->arp_notes.size())
                return this->arp_notes[arp_counter++];
            else
                return this->arp_notes[this->arp_notes.size() - (arp_counter++ - this->arp_notes.size() + 2)];
        case ArpType::PICKING_IN:
            arp_counter = arp_counter % this->arp_notes.size();
            if (this->counter % 2 == 0)
                return this->arp_notes[arp_counter++ / 2];
            else
                return this->arp_notes[this->arp_notes.size() - ++arp_counter / 2];
        case ArpType::CLOSEST:
            return getClosest(this->arp_notes, true, last_note);
        case ArpType::CLOSEST_EXC:
            return getClosest(this->arp_notes, false, last_note);
        case ArpType::RANDOM:
            return this->arp_notes[Rand::randui8(this->arp_notes.size())];
        }
        return 0;
    }

    void ArpData::get_arp_pitches_by_range(const Scale& scale, const uint8_t chord)
    {
        for (int i = this->min; i < this->min + this->range; i++)
        {
            if (scale.chord_contains(i, chord))
            {
                this->arp_notes.push_back(i);
            }
        }
    }

    void ArpData::get_arp_pitches_by_count(const Scale& scale, const uint8_t chord)
    {
        
        for (
            int i = this->min; 
            this->arp_notes.size() < this->range_count && i < 128; 
            i++)
        {
            if (scale.chord_contains(i, chord))
            {

                this->arp_notes.push_back(i);
            }
        }
    }

    uint8_t ArpData::get_next_arp_pitch(const Scale& scale, const uint8_t chord)
    {
        arp_notes = {};
        switch (this->range_type)
        {
        case RangeType::Range: this->get_arp_pitches_by_range(scale, chord); break;
        case RangeType::Count: this->get_arp_pitches_by_count(scale, chord); break;
        }

        return this->get_arp_pitch(this->counter, this->last_note);
    }

    uint8_t ArpData::get_arp_pitch() const
    {
        uint8_t counter = this->counter;
        uint8_t last_note = this->last_note;
        return get_arp_pitch(counter, last_note);
    }
}