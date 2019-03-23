#pragma once

#include "gate_patterns.h"
#include "interval_pattern.h"
#include "fugue_settings.h"

class MonoSettings : public InstrumentBase
{
protected:
    FugueSettings& fugue_settings;
    HarmonyStruct& harmony;
    TimeStruct& time;

public:
    MonoStyle style;
    ArpData arp_data;
    IntervalPattern int_pattern;
    GatePatternAB euclid_pattern;
    IntervalPattern lead_pattern;

    uint8_t variable_pitch_offset;
    uint8_t variable_density;

    uint8_t pitch_offset;
    uint8_t fugue_id;

    PitchStorage storage;

    MonoSettings(FugueSettings& fugue_settings_ref,
        HarmonyStruct& harmony_ref,
        TimeStruct& time_ref) :
        fugue_settings(fugue_settings_ref),
        harmony(harmony_ref),
        time(time_ref)
    {
        style = MonoStyle::MonoSixteenths;
        fugue_id = 0;
    }

    void randomize()
    {
        this->pitch_offset = randui8(24, 48);
        this->arp_data.range = randui8(12, 24);

        switch (randui8(4))
        {
        case 0: this->arp_data.type = ArpType::UP;
        case 1: this->arp_data.type = ArpType::DOWN;
        case 2: this->arp_data.type = ArpType::UPDOWN;
        case 3: this->arp_data.type = ArpType::PICKING_IN;
        }

        switch (randui8(3))
        {
        case 0: this->style = MonoStyle::MonoSixteenths; break;
        case 1: this->style = MonoStyle::MonoPolyRhythm; break;
        case 2: this->style = MonoStyle::MonoLeadPattern; break;
        }

        set_euclid(this->euclid_pattern, randui8(5, 8), 1);
        this->euclid_pattern.time_division = TimeDivision::Sixteenth;

        this->lead_pattern.randomize_interval_lead();
    }

    TimeDivision get_time_division() const
    {
        return this->variable_density < 32 ? TimeDivision::Quarter :
            this->variable_density < 64 ? TimeDivision::DottedEight :
            this->variable_density < 96 ? TimeDivision::Eight :
            TimeDivision::Sixteenth;
    }

    bool get_mono_hit() const
    {
        bool hit = false;
        switch (this->style)
        {
        case MonoStyle::MonoSixteenths:
        {
            hit = interval_hit(this->get_time_division(), time);
            break;
        }
        case MonoStyle::MonoPolyRhythm:
            hit = this->euclid_pattern.gate(time);
            break;
        case MonoStyle::MonoLeadPattern:
            hit = this->lead_pattern.hit(time);
            break;
        }
        return hit;
    }

    uint8_t get_next_mono_pitch()
    {
        this->arp_data.min = rerange(this->variable_pitch_offset, 48, this->pitch_offset);
        uint8_t pitch = get_next_arp_pitch(this->arp_data, harmony.scale, harmony.get_chord_step(time));
        return pitch;
    }

    uint8_t get_mono_pitch() const
    {
        return get_arp_pitch(this->arp_data);
    }

    void play()
    {
        if (this->style == MonoStyle::MonoFugue && !this->kill)
        {
            return play_fugue(
                this->fugue_settings,
                this->fugue_id,
                harmony,
                time,
                this->storage);
        }

        bool hit = this->get_mono_hit();

        if (hit)
        {
            uint8_t pitch = this->get_next_mono_pitch();

            uint8_t length = 6;
            if (this->style == MonoStyle::MonoLeadPattern)
            {
                length = time.ticks_left_in_bar();
            }
            else if (this->style == MonoStyle::MonoSixteenths)
            {
                length = this->get_time_division();
            }

            if (!this->kill)
            {
                this->storage.note_on(
                    make_note(pitch, 64, length, NoteType::Tie),
                    time.get_shuffle_delay()
                );
            }
        }
    }

};
