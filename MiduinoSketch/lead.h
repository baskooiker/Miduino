#pragma once

#include "arp.h"
#include "cv.h"
#include "intervals.h"
#include "midi_io.h"

void randomize_lead(ApplicationData& data)
{
    randomize_interval(data.settings_lead.int_pattern, arp_interval_probs);
    randomize(data.settings_lead.min_pitch_pattern, 60, 72);
    randomize_interval_lead(data.settings_lead.long_pattern);

    data.settings_lead.arp_data.type = ArpType::CLOSEST;
    data.settings_lead.arp_data.range = 12;
}

void play_lead(ApplicationData& data)
{
    bool hit = false;
    uint8_t length = 6;

    switch (data.settings_lead.style)
    {
    case LeadStyle::LeadSixteenths:
        hit = interval_hit(TimeDivision::TIME_DIVISION_SIXTEENTH, data.step, data.ticks);
        length = 3;
        break;
    case LeadStyle::LeadIntPattern:
        hit = interval_hit(data.settings_lead.int_pattern, data.step, data.ticks);
        length = TIE_NOTE;
        break;
    case LeadStyle::LeadLongPattern:
        hit = interval_hit(data.settings_lead.long_pattern, data.step, data.ticks);
        length = TIE_NOTE;
        break;
    }

    if (hit)
    {
        data.settings_lead.arp_data.min = cv(data.settings_lead.min_pitch_pattern, data.step);

        uint8_t chord = cv(data.harmony, data.step);
        uint8_t pitch = get_arp_pitch(data.settings_lead.arp_data, data.scale, chord);
        note_on(pitch, 64, MIDI_CHANNEL_LEAD, data.settings_lead.storage, length);
    }
}
