#include <MIDI.h>

#include "defs.h"

MIDI_CREATE_DEFAULT_INSTANCE();

uint8_t playing_pitches[16] = {};
uint8_t nr_playing_pitches = 0;

uint8_t midi_channel = 1;

#define CC_CUTOFF    74
#define CC_RESONANCE 71
#define CC_ENV_MOD   73
#define CC_OSC_WAVE  70
#define CC_OSC_TUNE  79

uint8_t params[] = {CC_CUTOFF, CC_RESONANCE, CC_ENV_MOD, CC_OSC_WAVE, CC_OSC_TUNE};
uint8_t nr_params = 5;

#define TICKS_PER_STEP 6

uint8_t counter = TICKS_PER_STEP;
bool play_step = false;

uint8_t step = 0;
#define MAX_NR_STEPS 256
const uint8_t nr_steps = 16;

uint8_t nr_pitches = 7;
uint8_t pitches[] = {36, 38, 39, 41, 43, 44, 46};

uint8_t pitch_seq[nr_steps]    = {36, 36, 38, 36, 39, 36, 41, 36, 41, 39, 36, 36, 36, 36, 36, 46};
uint8_t velocity_seq[nr_steps] = {64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64};
bool slide_seq[nr_steps]       = {};
bool gate_seq[nr_steps]        = {};

#define MIDI_CHANNEL_503 10

#define NOTE_503_BD 36
#define NOTE_503_SD 38
#define NOTE_503_LT 41
#define NOTE_503_MT 43
#define NOTE_503_HT 45 
#define NOTE_503_CY 46
#define NOTE_503_OH 39
#define NOTE_503_HH 40

static const uint8_t NR_BD_PATTERNS = 4;
static const uint8_t BD_PATTERNS[NR_BD_PATTERNS][16] = {
  {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0},
  {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0},
  {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0},
  {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0},
};

static const uint8_t NR_SD_PATTERNS = 4;
static const uint8_t SD_PATTERNS[NR_SD_PATTERNS][16] = {
  {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
  {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
  {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
};

static const uint8_t NR_HH_PATTERNS = 4;
static const uint8_t HH_PATTERNS[NR_SD_PATTERNS][16] = {
  {0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 2, 0},
  {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 2},
  {1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1},
  {1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0},
};

uint8_t* bd_pattern = (uint8_t*)BD_PATTERNS[0];
uint8_t* sd_pattern = (uint8_t*)SD_PATTERNS[0];
uint8_t* hh_pattern = (uint8_t*)HH_PATTERNS[0];

void setup() {
    MIDI.begin(MIDI_CHANNEL_OMNI);
    for (int i = 0; i < nr_steps; i++)
    {
      pitch_seq[i] = pitches[random(7)];
      velocity_seq[i] = random(0, 128);
      slide_seq[i] = false;
      gate_seq[i] = true;
    }
}

void stop_notes()
{
    while (nr_playing_pitches > 0)
    {
        MIDI.sendNoteOff(playing_pitches[--nr_playing_pitches], 0, midi_channel);
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
  
        // Full range random velocity
        velocity_seq[i] = random(0, 128);
  
        // 25% slide
        slide_seq[i] = random(128) < 32;
        
        // 80% slide
        gate_seq[i] = random(100) < 80;
    }
}

void randomize_503()
{
    bd_pattern = (uint8_t*)BD_PATTERNS[random(NR_BD_PATTERNS)];
    sd_pattern = (uint8_t*)SD_PATTERNS[random(NR_SD_PATTERNS)];
    hh_pattern = (uint8_t*)HH_PATTERNS[random(NR_HH_PATTERNS)];

    for (int i = 0; i < nr_random_503_params; i++)
    {
        RandomParam* p = &random_503_params[i];
        MIDI.sendControlChange(p->note, random(p->min, p->max), MIDI_CHANNEL_503);
    }
}

void play_step_rocket()
{
    uint8_t loc_step = step % 16;
    uint8_t pitch = pitch_seq[loc_step];
    uint8_t velocity = velocity_seq[loc_step];

    if (gate_seq[loc_step])
    {
        if (slide_seq[loc_step] == false)
        {
            stop_notes();
        }
        
        playing_pitches[nr_playing_pitches++] = pitch;
        MIDI.sendNoteOn(pitch, velocity, midi_channel);
    }
}

void play_503()
{
    uint8_t loc_step = step % 16;
    if (bd_pattern[loc_step] > 0)
    {
        MIDI.sendNoteOn(NOTE_503_BD, 63, MIDI_CHANNEL_503);
    }
    if (sd_pattern[loc_step] > 0)
    {
        MIDI.sendNoteOn(NOTE_503_SD, 63, MIDI_CHANNEL_503);
    }
    if (hh_pattern[loc_step] == 1)
    {
        MIDI.sendNoteOn(NOTE_503_HH, 63, MIDI_CHANNEL_503);
    }
    if (hh_pattern[loc_step] == 2)
    {
        MIDI.sendNoteOn(NOTE_503_OH, 63, MIDI_CHANNEL_503);
    }
}

void randomize_rocket_cc()
{
    for (int i = 0; i < nr_params; i++)
    {
      uint8_t cc = params[i];
      uint8_t value = random(0, 128);
      MIDI.sendControlChange(cc, value, midi_channel);
    }
}

void loop() {
    if (MIDI.read())
    {
        switch(MIDI.getType())
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
                step = 0;
                break;
            case midi::MidiType::ControlChange:
                switch(MIDI.getData1())
                {
                    case BSP_STEP_01:
                        randomize_rocket_seq();
                        randomize_503();
                        break;
                    default:
                        break;
                }
              break;
            case midi::MidiType::NoteOn:
                break;
            default:
                break;
        }
    }
    
    if (play_step)
    {
        play_503();
        play_step_rocket();
        
        play_step = false;
        step = (step + 1) % MAX_NR_STEPS;
    }
}

