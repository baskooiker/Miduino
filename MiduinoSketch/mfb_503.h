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

#endif // MFB_503_H
