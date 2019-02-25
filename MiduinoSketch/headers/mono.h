#pragma once

#include "fugue.h"
#include "harmony.h"
#include "rhythm_time.h"

void randomize_mono(MonoSettings& settings)
{
    settings.pitch_offset = randi(24, 48);
    settings.arp_data.range = randi(12, 24);

    switch (randi(4))
    {
    case 0: settings.arp_data.type = ArpType::UP;
    case 1: settings.arp_data.type = ArpType::DOWN;
    case 2: settings.arp_data.type = ArpType::UPDOWN;
    case 3: settings.arp_data.type = ArpType::PICKING_IN;
    }

    switch (randi(3))
    {
    case 0: settings.style = MonoStyle::MonoSixteenths; break;
    case 1: settings.style = MonoStyle::MonoPolyRhythm; break;
    case 2: settings.style = MonoStyle::MonoLeadPattern; break;
    }

    set_euclid(settings.euclid_pattern, randi(5, 8), 1);
    settings.euclid_pattern.time_division = TimeDivision::Sixteenth;

    randomize_interval_lead(settings.lead_pattern);
}

bool get_mono_hit(const MonoSettings& settings, const TimeStruct& time)
{
    bool hit = false;
    switch (settings.style)
    {
    case MonoStyle::MonoSixteenths:
        hit = interval_hit(TimeDivision::Sixteenth, time);
        break;
    case MonoStyle::MonoPolyRhythm:
        hit = gate(settings.euclid_pattern, time);
        break;
    case MonoStyle::MonoLeadPattern:
        hit = interval_hit(settings.lead_pattern, time);
        break;
    }
    return hit;
}

uint8_t get_next_mono_pitch(MonoSettings& settings, const HarmonyStruct& harmony, const TimeStruct& time)
{
    settings.arp_data.min = settings.pitch_offset
        + (uint8_t)(((uint16_t)settings.variable_pitch_offset * 24) / 128);
    uint8_t pitch = get_next_arp_pitch(settings.arp_data,
        harmony.scale,
        get_chord_step(harmony, time));
    return pitch;
}

uint8_t get_mono_pitch(const MonoSettings& settings, const HarmonyStruct& harmony, const TimeStruct& time)
{
    return get_arp_pitch(settings.arp_data);
}

void play_mono(
    ApplicationData& data,
    MonoSettings& settings, 
    const HarmonyStruct& harmony, 
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

        uint8_t length = 3;
        if (settings.style == MonoStyle::MonoLeadPattern)
        {
            length = 9;
        }

        note_on(make_note(pitch, 64, length, NoteType::Tie), settings.storage);
    }
}
