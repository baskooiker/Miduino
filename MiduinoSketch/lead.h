#pragma once

#include "arp.h"
#include "intervals.h"
#include "midi_io.h"

void randomize_lead(ApplicationData& data)
{
    // TODO
    randomize_interval(data.settings_lead.int_pattern, arp_interval_probs);
    randomize(data.settings_lead.min_pitch_pattern, 60, 72);
}

void play_lead(ApplicationData& data)
{
    bool hit = false;

    switch (data.settings_lead.style)
    {
    case LeadSixteenths: 
        hit = interval_hit(TimeDivision::TIME_DIVISION_SIXTEENTH, data.step, data.ticks);
        break;
    case LeadIntPattern:
        hit = interval_hit(data.settings_lead.int_pattern, data.step, data.ticks);
        break;
    }

    if (hit)
    {
        uint8_t chord = cv(data.harmony, data.step);
        uint8_t pitch = get_arp_pitch(data.settings_lead.arp_data, data.scale, chord, data.step, data.ticks);
        note_on(pitch, 64, MIDI_CHANNEL_LEAD, data.settings_lead.storage, 3);
    }
}
