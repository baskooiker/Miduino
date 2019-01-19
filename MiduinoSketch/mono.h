#pragma once

#include "rhythm_time.h"

void randomize_mono(ApplicationData& data)
{
    data.settings_mono.arp_data.min = randi(36, 60);
    data.settings_mono.arp_data.range = randi(12, 24);
    switch (randi(4))
    {
    case 0: data.settings_mono.arp_data.type = ArpType::UP;
    case 1: data.settings_mono.arp_data.type = ArpType::DOWN;
    case 2: data.settings_mono.arp_data.type = ArpType::UPDOWN;
    case 3: data.settings_mono.arp_data.type = ArpType::PICKING_IN;
    }

    switch (randi(3))
    {
    case 0: data.settings_mono.style = MonoStyle::Sixteenths; break;
    case 1: data.settings_mono.style = MonoStyle::PolyRhythm; break;
    case 2: data.settings_mono.style = MonoStyle::LeadPattern; break;
    }

    set_euclid(data.settings_mono.euclid_pattern, randi(5, 8), 1);

    randomize_interval_lead(data.settings_mono.lead_pattern);
}

void play_mono(ApplicationData& data)
{
    bool hit = false;
    switch (data.settings_mono.style)
    {
    case MonoStyle::Sixteenths: 
        hit = interval_hit(TimeDivision::TIME_DIVISION_SIXTEENTH, data.step, data.ticks); 
        break;
    case MonoStyle::PolyRhythm:
        hit = gate(data.settings_mono.euclid_pattern, data.step, data.ticks);
        break;
    case MonoStyle::LeadPattern:
        hit = interval_hit(data.settings_mono.lead_pattern, data.step, data.ticks);
        break;
    }

    if (hit)
    {
        uint8_t chord = cv(data.harmony, data.step);
        uint8_t pitch = get_arp_pitch(data.settings_mono.arp_data, data.scale, chord);
        //note_on(make_note(pitch, 64, ticks_left_in_bar(data.step, data.ticks)),
        note_on(make_note(pitch, 64, 3),
                MIDI_CHANNEL_MONO, 
                data.settings_mono.storage);
    }
}
