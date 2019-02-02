#pragma once

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

void play_mono(MonoSettings& settings, 
    const HarmonyStruct& harmony, 
    const uint32_t step, 
    const uint8_t tick)
{
    bool hit = false;
    switch (settings.style)
    {
    case MonoStyle::MonoSixteenths: 
        hit = interval_hit(TimeDivision::Sixteenth, step, tick); 
        break;
    case MonoStyle::MonoPolyRhythm:
        hit = gate(settings.euclid_pattern, step, tick);
        break;
    case MonoStyle::MonoLeadPattern:
        hit = interval_hit(settings.lead_pattern, step, tick);
        break;
    }

    if (hit)
    {
        settings.arp_data.min = settings.pitch_offset 
            + (uint8_t)(((uint16_t)settings.variable_pitch_offset * 24) / 128);
        uint8_t pitch = get_arp_pitch(settings.arp_data,
                                      harmony.scale, 
                                      get_chord_step(harmony, step, tick));

        uint8_t length = 3;
        if (settings.style == MonoStyle::MonoLeadPattern)
        {
            //length = ticks_left_in_bar(step, tick);
            length = 9;
        }

        note_on(make_note(pitch, 64, length, NoteType::Tie), settings.storage);
    }
}
