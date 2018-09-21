#include <TimerObject.h>

#include <MIDI.h>

#include "defs.h"
#include "rhythms.h"
#include "basslines.h"

#include "mfb_503.h"
#include "mfb_522.h"
#include "p50.h"

MIDI_CREATE_DEFAULT_INSTANCE();

uint8_t playing_pitches[16] = {};
uint8_t nr_playing_pitches = 0;

uint8_t counter = TICKS_PER_STEP;
bool play_step = false;

#define MAX_NR_STEPS 256
const uint8_t nr_steps = 16;

uint8_t nr_pitches = 7;
uint8_t pitches[] = {36, 38, 39, 41, 43, 44, 46};

uint8_t pitch_seq[nr_steps]    = {36, 36, 38, 36, 39, 36, 41, 36, 41, 39, 36, 36, 36, 36, 36, 46};
bool slide_seq[nr_steps]       = {};
bool gate_seq[nr_steps]        = {};

static RocketSettings rocket_settings = {};

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
    randomize_522_seq(&data);
    randomize_503_seq(&data);
    randomize_P50_seq(&data);
    randomize_rocket_seq();
  
    // Initialize settings
    rocket_settings.gate_density = 80;
  
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

void send_cc(uint8_t cc, uint8_t value, uint8_t channel)
{
    MIDI.sendControlChange(cc, value, channel);
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
            randomize_522_seq(&data);
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
    play_522(&data);
    play_step_rocket();
    play_P50(&data);

    play_step = false;
    data.step = (data.step + 1) % COMMON_DENOMINATOR;
  }
}

