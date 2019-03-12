#pragma once

#include "consts.h"
#include "enums.h"
#include "rand.h"

class TimeStruct 
{
public:
    uint32_t tick;
    PlayState state;
    uint32_t last_pulse_time;
    float average_pulse_time;
    uint8_t global_shuffle;

    TimeStruct()
    {
        tick = 0;
        state = PlayState::Stopped;
        last_pulse_time = 0;
        average_pulse_time = 500.f;
        global_shuffle = 0;
    }

    uint32_t step() const
    {
        return this->tick / TICKS_PER_STEP;
    }
};

class Coefficients {
public:
    float one;
    float two;
    float three;
    float four;
    float eights;
    float up;
    float down;
};

class RandomParam {
public:
  uint8_t note;
  uint8_t min;
  uint8_t max;  
};

#define STEPS_IN_BAR (16)
#define TICKS_IN_BAR (96)

class CvPattern
{
public:
    uint8_t pattern[STEPS_IN_BAR];

    void randomize(const uint8_t maximum = 128, const uint8_t minimum = 0)
    {
        for (int i = 0; i < STEPS_IN_BAR; i++)
        {
            this->pattern[i] = randui8(minimum, maximum);
        }
    }

    void set(const uint8_t i, const uint8_t value)
    {
        this->pattern[i] = value;
    }

    void set_all(const uint8_t value)
    {
        for (int i = 0; i < STEPS_IN_BAR; i++)
            this->pattern[i] = value;
    }

    uint8_t cv(const uint8_t step) const
    {
        return this->pattern[step % STEPS_IN_BAR];
    }
};

class CvPattern16 {
public:
    CvPattern pattern;
    uint8_t length;

    CvPattern16()
    {
        length = STEPS_IN_BAR;
    }

    void randomize(const uint8_t maximum = 128, const uint8_t minimum = 0)
    {
        this->pattern.randomize(maximum, minimum);
    }

    uint8_t cv(const TimeStruct& time)
    {
        return this->cv(time.step());
    }

    uint8_t cv(const uint8_t step)
    {
        return pattern.pattern[step % MAX(MIN(length, 16), 1)];
    }
};

class CvPatternAB 
{
public:
    CvPattern patterns[3];
    uint8_t abPattern[4];
    TimeDivision time_division;
    uint8_t length;

    CvPatternAB()
    {
        length = 64;
        time_division = TimeDivision::Sixteenth;
    }

    ~CvPatternAB() {}
};

typedef uint16_t BinaryPattern;

class GatePattern16
{
public:
    BinaryPattern pattern;
    uint8_t length;

    GatePattern16()
    {
        pattern = 0x00;
        length = 16;
    }

    ~GatePattern16() {}
};

 class GatePatternAB 
 {
 public:
    BinaryPattern patterns[3];
    uint8_t abPattern[4];
    TimeDivision time_division;
    uint8_t length;

    GatePatternAB()
    {
        patterns[0] = 0x00;
        patterns[1] = 0x00;
        patterns[2] = 0x00;
        length = 64;
        time_division = TimeDivision::Sixteenth;
    }

    ~GatePatternAB() {};
};

class NoteStruct 
{
public:
    uint8_t pitch;
    uint8_t velocity;
    uint8_t length;
    NoteType type;
};

class NoteEvent 
{
public:
    NoteStruct note;
    uint32_t time;
};

#define STORAGE_SIZE 8

class PitchStorage
{
public:
    NoteStruct data[STORAGE_SIZE];
    uint8_t size;

    NoteEvent events[STORAGE_SIZE];
    uint8_t nr_of_events;

    uint8_t channel;

    PitchStorage()
    {
        size = 0;
        nr_of_events = 0;
    }
};

class IntervalPattern {
public:
    TimeDivision pattern[16];
    TimeDivision time_division;
    uint8_t length;

    IntervalPattern()
    {
        length = 16;
        for (int i = 0; i < length; i++)
        {
            pattern[i] = TimeDivision::Sixteenth;
        }
        time_division = TimeDivision::Eight;
    }
};

class IntervalProbs 
{
public:
    uint8_t p_4;
    uint8_t p_8;
    uint8_t p_16;
    uint8_t p_32;
    uint8_t p_t8;
};

static const IntervalProbs arp_interval_probs = { 25, 25, 25, 0,  0 };

class ButtonState
{
public:
    unsigned long last_pressed;
    unsigned long last_released;
};

class UiState {
public:
    uint16_t bsp_button_state;

    ButtonState pad_state[NR_OF_PADS];
    ButtonState step_state[NR_OF_STEPS];
};

class FuguePlayerSettings {
public:
    uint8_t pitch_offset;
    uint8_t manual_pitch_offset;
    uint8_t length;
    FuguePlayerType type;
    uint32_t counter;
    uint8_t rhythm;
    uint8_t density;
    NoteInterval note_interval;
    uint8_t note_repeat;

    FuguePlayerSettings()
    {
        pitch_offset = 36;
        length = 4;
        type = FuguePlayerType::FugueForward;
        note_interval = NoteInterval::IntervalRoot;
        note_repeat = 1;
    }
};

#define NUMBER_FUGUE_PLAYERS 4

class FugueSettings {
public:
    CvPattern16 pattern;
    FuguePlayerSettings player_settings[NUMBER_FUGUE_PLAYERS];
};

class BassSettings 
{
public:
    GatePatternAB accents;
    CvPatternAB pitches;
    CvPatternAB octaves;
    CvPatternAB variable_octaves;
    GatePatternAB slides;

    CvPatternAB probs;
    IntervalPattern int_pattern;
    GatePatternAB euclid_pattern;
    GatePatternAB low_pattern;

    uint8_t pitch_range;
    BassStyle style;
    CvPatternAB note_range_prob;
    uint8_t note_range_value;
    uint8_t octave_offset;
    uint8_t fugue_id;
    uint8_t density;

    bool kill;

    PitchStorage storage;

    BassSettings()
    {
        pitch_range = 0;
        style = BassStyle::BassLow;
        note_range_value = 0;
        density = 0;
        octave_offset = 2;
        kill = false;
    }
};

class BassDubSettings {
public:
    BassDubStyle style;
    NoteInterval note_interval;
    GatePatternAB hit_probs;
    uint8_t density;
    uint8_t v_pitch;
    uint8_t fugue_id;
    PitchStorage storage;

    BassDubSettings()
    {
        style = BassDubStyle::DubOctave;
        note_interval = NoteInterval::IntervalRoot;
        density = 0;
        v_pitch = 0;
    }
};

class Mfb522Settings 
{
public:
    GatePattern16 ac_522_pattern;
    GatePatternAB bd_522_pattern;
    GatePattern16 lo_tom_522_pattern;
    GatePattern16 mi_tom_522_pattern;
    GatePattern16 rs_522_pattern;
    GatePatternAB clave_522_pattern;
    GatePatternAB clap_522_pattern;
    GatePatternAB hh_522_pattern;
    GatePatternAB oh_522_pattern;
    GatePatternAB cy_522_pattern;
    GatePatternAB sd_522_pattern;

    bool use_hh_int;
    IntervalPattern hh_int_pattern;

    PitchStorage storage;

    Mfb522Settings()
    {
        use_hh_int = false;
    }
};

class ArpData
{
public:
    uint8_t min;
    uint8_t range;
    uint8_t range_count;
    uint8_t counter;
    ArpType type;
    RangeType range_type;
    uint8_t last_note;

    uint8_t arp_notes[32];
    uint8_t arp_notes_length;

    ArpData()
    {
        min = 36;
        range = 12;
        range_count = 3;
        counter = 0;
        type = ArpType::UP;
        range_type = RangeType::Range;
        last_note = 0;

        arp_notes_length = 0;
    }
};

class PolySettings 
{
public:
    GatePatternAB gates_low;
    GatePatternAB gates;
    GatePatternAB tie_pattern;
    uint8_t pitch_offset;
    PolyType type;
    PitchStorage storage;

    PolySettings()
    {
        pitch_offset = 48;
        type = PolyType::PolyLow;
    }
};

class Mfb503Settings 
{
public:
    GatePatternAB bd_pattern;
    GatePatternAB sd_pattern;
    GatePatternAB hh_pattern;
    GatePatternAB oh_pattern;
    GatePatternAB cy_pattern;
    CvPatternAB tom_pattern;

    IntervalPattern hat_int_pattern;
    CvPatternAB hat_velocity;

    uint8_t bd_decay;
    bool play_pitch_bd;

    uint8_t volume_cy;

    uint8_t volume_tom;
    uint8_t nr_toms;
    uint8_t toms_offset;
    GatePatternAB tom_mask;

    HatClosedStyle hat_closed_style;
    uint8_t closed_hat_note;
    
    bool kill_low;
    bool kill_mid;
    bool kill_perc;
    bool kill_hats;
    bool drum_fill;
    uint8_t snare_roll;
    uint8_t bd_decay_factor;

    PitchStorage storage;

    Mfb503Settings()
    {
        hat_closed_style = HatClosedStyle::HatClosedRegular;
        volume_cy = 0;
        volume_tom = 0;
        kill_hats = false;
        closed_hat_note = NOTE_503_HH_1;
    }
};

class MicroTimingStruct 
{
public:
    int8_t shuffle_off;
    uint8_t delay;
};

class TanzbarTimeSettings 
{
public:
    MicroTimingStruct bd;
    MicroTimingStruct sd;
    MicroTimingStruct cp;
    MicroTimingStruct hh;
    MicroTimingStruct tc;
    MicroTimingStruct ma;
    MicroTimingStruct cl;
    MicroTimingStruct cb;
    MicroTimingStruct cy;
};

class TanzbarSettings {
public:
    GatePatternAB bd_pattern;
    GatePatternAB sd_pattern;
    GatePatternAB rs_pattern;
    GatePatternAB cp_pattern;
    GatePatternAB hh_pattern;
    GatePatternAB oh_pattern;
    GatePatternAB cy_pattern;
    GatePatternAB cl_pattern;
    GatePatternAB cb_pattern;
    CvPatternAB tom_pattern;
    CvPatternAB ma_pattern;

    IntervalPattern hat_int_pattern;
    CvPatternAB hat_velocity;

    uint8_t modulate_ma_range;
    uint8_t modulate_ma_offset;

    uint8_t toms_offset;
    PercussionType percussion_type;
    GatePatternAB tom_mask;

    HatClosedStyle hat_closed_style;

    bool kill_low;
    bool kill_mid;
    bool kill_perc;
    bool kill_hats;
    bool drum_fill;
    uint8_t snare_roll;

    TanzbarTimeSettings time_settings;

    PitchStorage storage;

    TanzbarSettings()
    {
        hat_closed_style = HatClosedStyle::HatClosedRegular;
        kill_hats = false;
        percussion_type = PercussionType::PercussionToms;
    }
};

class LeadSettings 
{
public:
    ArpData arp_data;
    CvPatternAB min_pitch_pattern;
    GatePatternAB pattern_slow;
    LeadStyle style;
    PitchStorage storage;

    LeadSettings()
    {
        style = LeadStyle::LeadSlow;
    }
};

class MonoSettings 
{
public:
    MonoStyle style;
    ArpData arp_data;
    IntervalPattern int_pattern;
    GatePatternAB euclid_pattern;
    IntervalPattern lead_pattern;

    uint8_t variable_pitch_offset;
    uint8_t variable_density;

    uint8_t pitch_offset;
    uint8_t fugue_id;

    PitchStorage storage;

    MonoSettings()
    {
        style = MonoStyle::MonoSixteenths;
        fugue_id = 0;
    }
};

class MonoDubSettings 
{
public:
    uint8_t variable_pitch_offset;
    MonoSettings settings;
    MonoDubStyle style;

    MonoDubSettings()
    {
        variable_pitch_offset = 0;
        style = MonoDubStyle::MonoDubLead;
    }
};
