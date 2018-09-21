#include <TimerObject.h>

#include <MIDI.h>

#include "defs.h"
#include "rhythms.h"
#include "basslines.h"

#include "mfb_503.h"
#include "p50.h"

MIDI_CREATE_DEFAULT_INSTANCE();

void randomize_522_seq();

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
  {SD_NOISE ,   0, 127},

  {HH_LEVEL , 100, 127},
  {HH_MIX   ,   0, 127},
  {OH_DECAY ,   0, 127},
  {HH_DECAY ,   0, 127},
};
uint8_t nr_random_503_params = sizeof(random_503_params) / sizeof(RandomParam);

uint8_t playing_pitches[16] = {};
uint8_t nr_playing_pitches = 0;

uint8_t params[] = {CC_CUTOFF, CC_RESONANCE, CC_ENV_MOD, CC_OSC_WAVE, CC_OSC_TUNE};
uint8_t nr_params = 5;

uint8_t counter = TICKS_PER_STEP;
bool play_step = false;

#define MAX_NR_STEPS 256
const uint8_t nr_steps = 16;

uint8_t nr_pitches = 7;
uint8_t pitches[] = {36, 38, 39, 41, 43, 44, 46};

uint8_t pitch_seq[nr_steps]    = {36, 36, 38, 36, 39, 36, 41, 36, 41, 39, 36, 36, 36, 36, 36, 46};
bool slide_seq[nr_steps]       = {};
bool gate_seq[nr_steps]        = {};

uint8_t* bd_pattern = (uint8_t*)BD_PATTERNS[0];
uint8_t* sd_pattern = (uint8_t*)SD_PATTERNS[0];
uint8_t* hh_pattern = (uint8_t*)HH_PATTERNS[0];

RhythmPattern perc_pattern_1 = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  16
};

RhythmPattern perc_pattern_2 = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  16
};

RhythmPattern perc_pattern_3 = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  16
};

GatePattern64 perc_pattern_clave;
GatePattern16 clap_pattern;

static RocketSettings rocket_settings = {};

static Mfb522Settings mfb_522_settings = {};

static ApplicationData data = {};

void setup() {
    MIDI.begin(MIDI_CHANNEL_OMNI);
    for (int i = 0; i < nr_steps; i++)
    {
      pitch_seq[i] = pitches[random(7)];
      slide_seq[i] = false;
      gate_seq[i] = true;
    }
  
    // Initialize patterns
    randomize_522_seq();
    randomize_503_seq(&data);
    randomize_P50_seq();
    randomize_rocket_seq();
  
    uint8_t clap_pat[] = {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0};
    clap_pattern = init_pattern(clap_pat, 16);
  
    // Initialize settings
    rocket_settings.gate_density = 80;
  
    mfb_522_settings.perc_midi_1 = NOTE_522_LO_TOM;
    mfb_522_settings.perc_midi_2 = NOTE_522_MI_TOM;
    mfb_522_settings.perc_midi_3 = NOTE_522_RS;

    data.rocket_octaves = 3;
    data.p50_octaves = 3;
    data.root = ROOT_C;
    data.scale = AEOLIAN;

    init_storage(data.storage_p50);
}

void note_on(uint8_t note, uint8_t velocity, uint8_t channel, uint8_t* storage)
{
    uint8_t stored = pop_from_storage(storage, note);
    if (stored > 0)
    {
        MIDI.sendNoteOff(note, 0, channel);
    }
    MIDI.sendNoteOn(note, velocity, channel);
    add_to_storage(storage, note);
}

void note_off(uint8_t note, uint8_t channel, uint8_t* storage)
{
    MIDI.sendNoteOff(note, 0, channel);
    uint8_t stored = pop_from_storage(storage, note);
}
void stop_notes()
{
  while (nr_playing_pitches > 0)
  {
    MIDI.sendNoteOff(playing_pitches[--nr_playing_pitches], 0, MIDI_CHANNEL_ROCKET);
  }
}

void randomize_rocket_seq()
{
  for (int i = 0; i < nr_steps; i++)
  {
    // 25% of pitch change
    if (random(128) < 32)
    {
      pitch_seq[i] = pitches[random(7)] + (random(3) - 1) * 12;
    }
    randomize_ab(&data.rocket_pattern.slides, .25f);
    // 25% slide
    slide_seq[i] = random(128) < 32;

    // 80% slide
    gate_seq[i] = random(127) < rocket_settings.gate_density;
  }
  data.rocket_pattern.accents = init_percussive_pattern_64();
}

void randomize_503_sound()
{
  for (int i = 0; i < nr_random_503_params; i++)
  {
    RandomParam* p = &random_503_params[i];
    MIDI.sendControlChange(p->note, random(p->min, p->max), MIDI_CHANNEL_503);
  }
}

//void randomize_503_seq()
//{
//  bd_pattern = (uint8_t*)BD_PATTERNS[random(NR_BD_PATTERNS)];
//  sd_pattern = (uint8_t*)SD_PATTERNS[random(NR_SD_PATTERNS)];
//  hh_pattern = (uint8_t*)HH_PATTERNS[random(NR_HH_PATTERNS)];
//}

void randomize_522_seq()
{
  for (int i = 0; i < 16; i++)
  {
    perc_pattern_1.data[i] = random(100) < 25;
    perc_pattern_2.data[i] = random(100) < 25;
    perc_pattern_3.data[i] = random(100) < 25;
  }
  perc_pattern_clave = init_percussive_pattern_64();
}

void play_step_rocket()
{
  uint8_t loc_step = data.step % 16;
  uint8_t pitch = pitch_seq[loc_step];
  Bassline* rocket = &data.rocket_pattern;

  uint8_t velocity = 32;
  if (gate(rocket->accents, data.step))
  {
    velocity = 100;
  }

  if (gate_seq[loc_step])
  {
    if (slide_seq[loc_step] == false)
    {
      stop_notes();
    }

    playing_pitches[nr_playing_pitches++] = pitch;
    MIDI.sendNoteOn(pitch, velocity, MIDI_CHANNEL_ROCKET);
  }
}

//void play_503()
//{
//  uint8_t loc_step = data.step % 16;
//  if (bd_pattern[loc_step] > 0)
//  {
//    MIDI.sendNoteOn(NOTE_503_BD, 63, MIDI_CHANNEL_503);
//  }
//  if (sd_pattern[loc_step] > 0)
//  {
//    MIDI.sendNoteOn(NOTE_503_SD, 63, MIDI_CHANNEL_503);
//  }
//  if (hh_pattern[loc_step] == 1)
//  {
//    MIDI.sendNoteOn(NOTE_503_HH, 63, MIDI_CHANNEL_503);
//  }
//  if (hh_pattern[loc_step] == 2)
//  {
//    MIDI.sendNoteOn(NOTE_503_OH, 63, MIDI_CHANNEL_503);
//  }
//}

void play_522()
{
  long loc_step = data.step % 16;
  uint8_t velocity = 63;
  if (perc_pattern_1.data[loc_step] > 0)
  {
    MIDI.sendNoteOn(mfb_522_settings.perc_midi_1, velocity, MIDI_CHANNEL_522);
  }
  if (perc_pattern_2.data[loc_step] > 0)
  {
    MIDI.sendNoteOn(mfb_522_settings.perc_midi_2, velocity, MIDI_CHANNEL_522);
  }
  if (perc_pattern_3.data[loc_step] > 0)
  {
    MIDI.sendNoteOn(mfb_522_settings.perc_midi_3, velocity, MIDI_CHANNEL_522);
  }
  if (gate(perc_pattern_clave, data.step))
  {
    MIDI.sendNoteOn(NOTE_522_CLAVE, velocity, MIDI_CHANNEL_522);
  }
  if (gate(clap_pattern, data.step))
  {
    MIDI.sendNoteOn(NOTE_522_CP_LONG, velocity, MIDI_CHANNEL_522);
  }
}

void randomize_rocket_cc()
{
  for (int i = 0; i < nr_params; i++)
  {
    uint8_t cc = params[i];
    uint8_t value = random(0, 128);
    MIDI.sendControlChange(cc, value, MIDI_CHANNEL_ROCKET);
  }
}

void randomize_P50_seq()
{
    data.p50_pattern = init_chord_pattern();
}

//void play_P50()
//{ 
//    ChordPattern* pattern = &data.p50_pattern;
//    uint8_t velocity = 32;
//    if (gate(pattern->gates, step))
//    {
//        uint8_t p = 0;
//        do {
//            p = pop_from_storage(data.storage_p50);
//            if (p > 0)
//            {
//                MIDI.sendNoteOff(p, 0, MIDI_CHANNEL_P50);
//            }
//        } while (p != 0);
//
//        uint8_t root = pitch(&pattern->pitches, step % 64);
//        uint8_t fifth = root + 7;
////        uint8_t third = next_in_scale(root, data.root, data.scale, 2);
////        uint8_t fifth = next_in_scale(root, data.root, data.scale, 4);
//        MIDI.sendNoteOn(root , velocity, MIDI_CHANNEL_P50);
////        MIDI.sendNoteOn(third, velocity, MIDI_CHANNEL_P50);
//        MIDI.sendNoteOn(fifth, velocity, MIDI_CHANNEL_P50);
//        
//        add_to_storage(data.storage_p50, root);
////        add_to_storage(data.storage_p50, third);
//        add_to_storage(data.storage_p50, fifth);
//    }
//}

void loop() {
  if (MIDI.read())
  {
    switch (MIDI.getType())
    {
      case midi::MidiType::Clock:
        counter = (counter + 1) % TICKS_PER_STEP;
        if (counter == 0)
        {
          play_step = true;
        }
        break;
      case midi::MidiType::Stop:
        stop_notes();
        data.step = 0;
        break;
      case midi::MidiType::ControlChange:
        switch (MIDI.getData1())
        {
          case BSP_KNOB_08:
            rocket_settings.gate_density = MIDI.getData2();
            break;
          case BSP_STEP_01:
            if (MIDI.getData2() == 0)
            {
              randomize_503_sound();
            }
            break;
          case BSP_STEP_02:
            if (MIDI.getData2() == 0)
            {
              randomize_503_seq(&data);
            }
            break;
          case BSP_STEP_03:
            randomize_522_seq();
            break;
          case BSP_STEP_15:
          case BSP_STEP_16:
            if (MIDI.getData2() == 0)
            {
              randomize_rocket_seq();
            }
            break;
          default:
            break;
        }
        break;
      case midi::MidiType::NoteOn:
        switch (MIDI.getData1())
        {
          case BSP_PAD_01:
            data.bsp_pad_01_down = true;
            break;
          case BSP_PAD_02:
            data.bsp_pad_02_down = true;
            break;
          case BSP_PAD_03:
            data.bsp_pad_03_down = true;
            break;
          case BSP_PAD_04:
            data.bsp_pad_04_down = true;
            break;
          case BSP_PAD_05:
            data.bsp_pad_05_down = true;
            break;
          case BSP_PAD_06:
            data.bsp_pad_06_down = true;
            break;
          case BSP_PAD_07:
            data.bsp_pad_07_down = true;
            break;
          case BSP_PAD_08:
            data.bsp_pad_08_down = true;
            break;
          case BSP_PAD_09:
            data.bsp_pad_09_down = true;
            break;
          case BSP_PAD_10:
            data.bsp_pad_10_down = true;
            break;
          case BSP_PAD_11:
            data.bsp_pad_11_down = true;
            break;
          case BSP_PAD_12:
            data.bsp_pad_12_down = true;
            break;
          case BSP_PAD_13:
            data.bsp_pad_13_down = true;
            break;
          case BSP_PAD_14:
            data.bsp_pad_14_down = true;
            break;
          case BSP_PAD_15:
            data.bsp_pad_15_down = true;
            break;
          case BSP_PAD_16:
            data.bsp_pad_16_down = true;
            break;
          default:
            break;
        }
        break;
      case midi::MidiType::NoteOff:
        switch (MIDI.getData1())
        {
          case BSP_PAD_01:
            data.bsp_pad_01_down = false;
            break;
          case BSP_PAD_02:
            data.bsp_pad_02_down = false;
            break;
          case BSP_PAD_03:
            data.bsp_pad_03_down = false;
            break;
          case BSP_PAD_04:
            data.bsp_pad_04_down = false;
            break;
          case BSP_PAD_05:
            data.bsp_pad_05_down = false;
            break;
          case BSP_PAD_06:
            data.bsp_pad_06_down = false;
            break;
          case BSP_PAD_07:
            data.bsp_pad_07_down = false;
            break;
          case BSP_PAD_08:
            data.bsp_pad_08_down = false;
            break;
          case BSP_PAD_09:
            data.bsp_pad_09_down = false;
            break;
          case BSP_PAD_10:
            data.bsp_pad_10_down = false;
            break;
          case BSP_PAD_11:
            data.bsp_pad_11_down = false;
            break;
          case BSP_PAD_12:
            data.bsp_pad_12_down = false;
            break;
          case BSP_PAD_13:
            data.bsp_pad_13_down = false;
            break;
          case BSP_PAD_14:
            data.bsp_pad_14_down = false;
            break;
          case BSP_PAD_15:
            data.bsp_pad_15_down = false;
            break;
          case BSP_PAD_16:
            data.bsp_pad_16_down = false;
            break;
          default:
            break;
        }
        break;
      default:
        break;
    }
  }

  if (play_step)
  {
    play_503(&data);
    play_522();
    play_step_rocket();
    play_P50(&data);

    play_step = false;
    data.step = (data.step + 1) % COMMON_DENOMINATOR;
  }
}

