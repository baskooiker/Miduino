#pragma once

#include "coef.h"
#include "cv.h"
#include "defs.h"
#include "euclid.h"
#include "gate.h"
#include "intervals.h"
#include "midi_io.h"
#include "rhythms.h"

void randomize_503_seq(ApplicationData& data)
{
    set_euclid(data.settings_503.ac_pattern.pattern, 16, randi(4, 13));

    set_coef_kick_pattern(data.settings_503.bd_pattern);

    data.settings_503.sd_pattern = init_pattern(SD_PATTERNS[randi(NR_SD_PATTERNS)], 16);

    if (randf() < .5)
    {
        set_coef_hat_pattern(data.settings_503.oh_pattern);
        set_random_pattern_ab(data.settings_503.hh_pattern, 0.f);
    }
    else
    {
        set_coef_hat_pattern(data.settings_503.hh_pattern);
        set_random_pattern_ab(data.settings_503.oh_pattern, 0.f);
    }
}

void play_fill(ApplicationData& data)
{
    static TimeDivision division = TimeDivision::TIME_DIVISION_SIXTEENTH;
    if (interval_hit(TimeDivision::TIME_DIVISION_TRIPLE_EIGHT, data.step, data.ticks))
    {
        uint8_t r = randi(16);
        if (r < 3)
        {
            division = TimeDivision::TIME_DIVISION_THIRTYTWO;
        }
        else
        {
            division = TimeDivision::TIME_DIVISION_SIXTEENTH;
        }
    }

    if (!interval_hit(division, data.step, data.ticks)) 
        return;

    uint8_t p = 0;
    switch (randi(7))
    {
    case 0: p = NOTE_503_BD; break;
    case 1: p = NOTE_503_SD; break;
    case 2: p = NOTE_503_LT; break;
    case 3: p = NOTE_503_MT; break;
    case 4: p = NOTE_503_HT; break;
    case 5: p = NOTE_503_OH; break;
    case 6: p = NOTE_503_HH; break;
    }
    note_on(p, 127, MIDI_CHANNEL_503, data.settings_503.storage);
}

void play_roll(ApplicationData& data)
{

    static TimeDivision division = TimeDivision::TIME_DIVISION_SIXTEENTH;
    if (interval_hit(TimeDivision::TIME_DIVISION_TRIPLE_EIGHT, data.step, data.ticks))
    {
        uint8_t r = randi(16);
        if (r < 3)
        {
            division = TimeDivision::TIME_DIVISION_THIRTYTWO;
        }
        else
        {
            division = TimeDivision::TIME_DIVISION_SIXTEENTH;
        }
    }

    if (interval_hit(division, data.step, data.ticks))
    {
        uint8_t velocity = 127;
        note_on(NOTE_503_SD, velocity, MIDI_CHANNEL_503, data.settings_503.storage);
    }
}

void play_503(ApplicationData& data)
{
    if (data.uiState.drum_fill)
    {
        return play_fill(data);
    }

    if (data.uiState.drum_roll)
    {
        play_roll(data);
    }

    uint8_t velocity = 63;
    if (gate(data.settings_503.ac_pattern, data.step, data.ticks))
    {
        velocity = 127;
    }

    if (gate(data.settings_503.bd_pattern, data.step, data.ticks) && !data.uiState.kill_low)
    {
        note_on(NOTE_503_BD, 127, MIDI_CHANNEL_503, data.settings_503.storage);
    }
    if (gate(data.settings_503.sd_pattern, data.step, data.ticks) && !data.uiState.kill_mid)
    {
        note_on(NOTE_503_SD, velocity, MIDI_CHANNEL_503, data.settings_503.storage);
    }

    bool hh = gate(data.settings_503.hh_pattern, data.step, data.ticks);
    if (gate(data.settings_503.oh_pattern, data.step, data.ticks) && !hh && !data.uiState.kill_high)
    {
        note_on(NOTE_503_OH, velocity, MIDI_CHANNEL_503, data.settings_503.storage);
    }
    if (hh && !data.uiState.kill_high)
    {
        note_on(NOTE_503_HH, velocity, MIDI_CHANNEL_503, data.settings_503.storage);
    }
}

const RandomParam random_503_params[] = {
  {BD_LEVEL , 126, 127},
  {BD_TUNE  ,  16, 64 },
  {BD_PITCH ,  16, 64 },
  {BD_DRIVE ,   0, 127},
  {BD_ATTACK,   0, 127},

  {SD_LEVEL , 126, 127},
  {SD_TUNE  ,   0, 127},
  {SD_DECAY ,   0, 64},
  {SD_NOISE ,  64, 127},

  {HH_LEVEL , 126, 127},
  {HH_MIX   ,   0, 127},
  {OH_DECAY ,   0, 64 },
  {HH_DECAY ,   0, 64 },
};
const uint8_t nr_random_503_params = sizeof(random_503_params) / sizeof(RandomParam);

void randomize_503_sound()
{
    for (int i = 0; i < nr_random_503_params; i++)
    {
        RandomParam* p = &random_503_params[i];
        send_cc(p->note, randi(p->min, p->max), MIDI_CHANNEL_503);
    }
}
