#ifndef MFB_503_H
#define MFB_503_H

#include "defs.h"
#include "midi_io.h"
#include "rhythms.h"

void randomize_503_seq(ApplicationData* data)
{
    data->bd_503_pattern = init_pattern(BD_PATTERNS[random(NR_BD_PATTERNS)], 16);
    data->sd_503_pattern = init_pattern(SD_PATTERNS[random(NR_SD_PATTERNS)], 16);

    uint8_t hh_idx = random(NR_HH_PATTERNS);
    uint8_t oh_p[16];
    for (int i = 0; i < 16; i++)
    {
        set_gate(&data->hh_503_pattern.pattern, i, HH_PATTERNS[hh_idx][i] == 1);
        set_gate(&data->oh_503_pattern.pattern, i, HH_PATTERNS[hh_idx][i] == 2);
    }
}

void play_503(ApplicationData* data)
{
  uint8_t loc_step = data->step % 16;
  uint8_t velocity = 63;
  if (gate(data->bd_503_pattern.pattern, data->step))
  {
    note_on(NOTE_503_BD, velocity, MIDI_CHANNEL_503, data->storage_503);
  }
  if (gate(data->sd_503_pattern.pattern, data->step))
  {
    note_on(NOTE_503_SD, velocity, MIDI_CHANNEL_503, data->storage_503);
  }
  boolean oh = gate(data->oh_503_pattern.pattern, data->step);
  if (gate(data->oh_503_pattern.pattern, data->step) && !oh)
  {
    note_on(NOTE_503_HH, velocity, MIDI_CHANNEL_503, data->storage_503);
  }
  if (oh)
  {
    note_on(NOTE_503_OH, velocity, MIDI_CHANNEL_503, data->storage_503);
  }
}

RandomParam random_503_params[] = {
  {BD_LEVEL , 100, 127},
  {BD_TUNE  ,   0, 64 },
  {BD_DECAY ,   0, 127},
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
        send_cc(p->note, random(p->min, p->max), MIDI_CHANNEL_503);
    }
}

#endif // MFB_503_H
