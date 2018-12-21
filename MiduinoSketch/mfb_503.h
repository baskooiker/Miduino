#pragma once

#include "coef.h"
#include "defs.h"
#include "euclid.h"
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

void play_503(ApplicationData& data)
{
    uint8_t velocity = 63;
    if (gate(data.settings_503.ac_pattern, data.step))
    {
        velocity = 127;
    }

    if (gate(data.settings_503.bd_pattern, data.step) && !data.uiState.kill_low)
    {

        note_on(NOTE_503_BD, 127, MIDI_CHANNEL_503, data.settings_503.storage);
    }
    if (gate(data.settings_503.sd_pattern, data.step) && !data.uiState.kill_mid)
    {
        note_on(NOTE_503_SD, velocity, MIDI_CHANNEL_503, data.settings_503.storage);
    }

    bool oh = gate(data.settings_503.oh_pattern, data.step);
    if (gate(data.settings_503.hh_pattern, data.step) && !oh && !data.uiState.kill_high)
    {
        note_on(NOTE_503_HH, velocity, MIDI_CHANNEL_503, data.settings_503.storage);
    }
    if (oh && !data.uiState.kill_high)
    {
        note_on(NOTE_503_OH, velocity, MIDI_CHANNEL_503, data.settings_503.storage);
    }
}

RandomParam random_503_params[] = {
  {BD_LEVEL , 100, 127},
  {BD_TUNE  ,   0, 64 },
  //{BD_DECAY ,   0, 127},
  {BD_PITCH ,   0, 64 },
  {BD_DRIVE ,   0, 127},
  {BD_ATTACK,   0, 127},

  {SD_LEVEL , 100, 127},
  {SD_TUNE  ,   0, 127},
  {SD_DECAY ,   0, 127},
  {SD_NOISE ,  64, 127},

  {HH_LEVEL , 100, 127},
  {HH_MIX   ,   0, 127},
  {OH_DECAY ,   0, 127},
  {HH_DECAY ,   0, 127},
};
uint8_t nr_random_503_params = sizeof(random_503_params) / sizeof(RandomParam);

void randomize_503_sound()
{
    for (int i = 0; i < nr_random_503_params; i++)
    {
        RandomParam* p = &random_503_params[i];
        send_cc(p->note, randi(p->min, p->max), MIDI_CHANNEL_503);
    }
}
