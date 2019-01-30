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
    randomize(data.lead_settings.min_pitch_pattern, 60, 78);
    switch (randi(3))
    {
    case 0: data.lead_settings.min_pitch_pattern.time_division = TimeDivision::Quarter; break;
    case 1: data.lead_settings.min_pitch_pattern.time_division = TimeDivision::Half; break;
    case 2: data.lead_settings.min_pitch_pattern.time_division = TimeDivision::Whole; break;
    }

    switch (distribution(10, 30))
    {
    case 0: data.lead_settings.arp_data.type = ArpType::CLOSEST; break;
    case 1: data.lead_settings.arp_data.type = ArpType::CLOSEST_EXC; break;
    }
    data.lead_settings.arp_data.range = 12;

    set_coef_slow_pattern(data.lead_settings.pattern_slow);
}

void play_lead(ApplicationData& data)
{
    bool hit = false;
    uint8_t length = 6;

    switch (data.lead_settings.style)
    {
    case LeadStyle::LeadWhole:
        hit = interval_hit(TimeDivision::Whole, data.step, data.ticks);
        length = TICKS_IN_BAR;
        break;
    case LeadStyle::LeadSlow:
        hit = gate(data.lead_settings.pattern_slow, data.step, data.ticks);
        length = ticks_left_in_bar(data.step, data.ticks);
        break;
    }

    if (hit)
    {
        data.lead_settings.arp_data.min = cv(data.lead_settings.min_pitch_pattern, data.step);

        uint8_t chord = get_chord_step(data.harmony, data.scale, data.step, data.ticks);
        uint8_t pitch = get_arp_pitch(data.lead_settings.arp_data, data.scale, chord);
        note_on(make_note(pitch, 64, length), data.lead_settings.storage);
    }
}
