#pragma once

#include "arp.h"
#include "coef.h"
#include "cv.h"
#include "harmony.h"
#include "intervals.h"
#include "midi_io.h"
#include "rhythm_time.h"

void randomize_lead(ApplicationData& data)
{
    randomize(data.settings_lead.min_pitch_pattern, 60, 78);
    switch (randi(3))
    {
    case 0: data.settings_lead.min_pitch_pattern.time_division = TimeDivision::TIME_DIVISION_FOURTH; break;
    case 1: data.settings_lead.min_pitch_pattern.time_division = TimeDivision::TIME_DIVISION_HALF; break;
    case 2: data.settings_lead.min_pitch_pattern.time_division = TimeDivision::TIME_DIVISION_WHOLE; break;
    }

    switch (distribution(10, 30))
    {
    case 0: data.settings_lead.arp_data.type = ArpType::CLOSEST; break;
    case 1: data.settings_lead.arp_data.type = ArpType::CLOSEST_EXC; break;
    }
    data.settings_lead.arp_data.range = 12;

    set_coef_slow_pattern(data.settings_lead.pattern_slow);
}

void play_lead(ApplicationData& data)
{
    bool hit = false;
    uint8_t length = 6;

    switch (data.settings_lead.style)
    {
    case LeadStyle::LeadWhole:
        hit = interval_hit(TimeDivision::TIME_DIVISION_WHOLE, data.step, data.ticks);
        length = TICKS_IN_BAR;
        break;
    case LeadStyle::LeadSlow:
        hit = gate(data.settings_lead.pattern_slow, data.step, data.ticks);
        length = ticks_left_in_bar(data.step, data.ticks);
        break;
    }

    if (hit)
    {
        data.settings_lead.arp_data.min = cv(data.settings_lead.min_pitch_pattern, data.step);

        uint8_t chord = get_chord_step(data);
        uint8_t pitch = get_arp_pitch(data.settings_lead.arp_data, data.scale, chord);
        note_on(make_note(pitch, 64, length), MIDI_CHANNEL_LEAD, data.settings_lead.storage);
    }
}
