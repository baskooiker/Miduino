#pragma once

#include "mono_settings.h"

#include "fugue.h"
#include "harmony.h"
#include "rhythm_time.h"

void randomize_mono(MonoSettings& settings)
{
    settings.pitch_offset = randui8(24, 48);
    settings.arp_data.range = randui8(12, 24);

    switch (randui8(4))
    {
    case 0: settings.arp_data.type = ArpType::UP;
    case 1: settings.arp_data.type = ArpType::DOWN;
    case 2: settings.arp_data.type = ArpType::UPDOWN;
    case 3: settings.arp_data.type = ArpType::PICKING_IN;
    }

    switch (randui8(3))
    {
    case 0: settings.style = MonoStyle::MonoSixteenths; break;
    case 1: settings.style = MonoStyle::MonoPolyRhythm; break;
    case 2: settings.style = MonoStyle::MonoLeadPattern; break;
    }

    set_euclid(settings.euclid_pattern, randui8(5, 8), 1);
    settings.euclid_pattern.time_division = TimeDivision::Sixteenth;

    settings.lead_pattern.randomize_interval_lead();
}

TimeDivision get_time_division(const MonoSettings& settings)
{
    return settings.variable_density < 32 ? TimeDivision::Quarter :
        settings.variable_density < 64 ? TimeDivision::DottedEight :
        settings.variable_density < 96 ? TimeDivision::Eight :
        TimeDivision::Sixteenth;
}

bool get_mono_hit(const MonoSettings& settings, const TimeStruct& time)
{
    bool hit = false;
    switch (settings.style)
    {
    case MonoStyle::MonoSixteenths:
    {            
        hit = interval_hit(get_time_division(settings), time);
        break;
    }
    case MonoStyle::MonoPolyRhythm:
        hit = settings.euclid_pattern.gate(time);
        break;
    case MonoStyle::MonoLeadPattern:
        hit = settings.lead_pattern.hit(time);
        break;
    }
    return hit;
}

uint8_t get_next_mono_pitch(MonoSettings& settings, const HarmonyStruct& harmony, const TimeStruct& time)
{
    settings.arp_data.min = rerange(settings.variable_pitch_offset, 48, settings.pitch_offset);
    uint8_t pitch = get_next_arp_pitch(settings.arp_data, harmony.scale, harmony.get_chord_step(time));
    return pitch;
}

uint8_t get_mono_pitch(const MonoSettings& settings, const HarmonyStruct& harmony, const TimeStruct& time)
{
    return get_arp_pitch(settings.arp_data);
}

void play_mono(
    ApplicationData& data,
    MonoSettings& settings, 
    HarmonyStruct& harmony, 
    const TimeStruct& time)
{
    if (settings.style == MonoStyle::MonoFugue)
    {
        return play_fugue(
            data.fugue_settings,
            settings.fugue_id, 
            harmony, 
            time, 
            settings.storage);
    }

    bool hit = get_mono_hit(settings, time);

    if (hit)
    {
        uint8_t pitch = get_next_mono_pitch(settings, harmony, time);

        uint8_t length = 6;
        if (settings.style == MonoStyle::MonoLeadPattern)
        {
            length = ticks_left_in_bar(time);
        }
        else if (settings.style == MonoStyle::MonoSixteenths)
        {
            length = get_time_division(settings);
        }

        if (!settings.kill)
        {
            settings.storage.note_on(
                make_note(pitch, 64, length, NoteType::Tie),
                time.get_shuffle_delay()
            );
        }
    }
}
