#pragma once

#include "lead_settings.h"

#include "arp.h"
#include "coef.h"
#include "cv.h"
#include "harmony.h"
#include "intervals.h"
#include "midi_io.h"
#include "rhythm_time.h"

void randomize_lead(LeadSettings& settings)
{
    settings.min_pitch_pattern.randomize(60, 78);
    switch (randui8(3))
    {
    case 0: settings.min_pitch_pattern.time_division = TimeDivision::Quarter; break;
    case 1: settings.min_pitch_pattern.time_division = TimeDivision::Half; break;
    case 2: settings.min_pitch_pattern.time_division = TimeDivision::Whole; break;
    }

    switch (distribution(10, 30))
    {
    case 0: settings.arp_data.type = ArpType::CLOSEST; break;
    case 1: settings.arp_data.type = ArpType::CLOSEST_EXC; break;
    }
    settings.arp_data.range = 12;

    set_coef_slow_pattern(settings.pattern_slow);
}

void play_lead(LeadSettings& settings, HarmonyStruct& harmony, const TimeStruct& time)
{
    bool hit = false;
    uint8_t length = 6;

    switch (settings.style)
    {
    case LeadStyle::LeadWhole:
        hit = interval_hit(TimeDivision::Whole, time);
        length = TICKS_IN_BAR;
        break;
    case LeadStyle::LeadSlow:
        hit = gate(settings.pattern_slow, time);
        length = ticks_left_in_bar(time);
        break;
    }

    if (hit)
    {
        settings.arp_data.min = settings.min_pitch_pattern.cv(time);

        uint8_t chord = get_chord_step(harmony, time);
        uint8_t pitch = get_next_arp_pitch(settings.arp_data, harmony.scale, chord);
        note_on(make_note(pitch, 64, length), settings.storage);
    }
}
