#pragma once

#include "coef.h"
#include "defs.h"
#include "euclid.h"
#include "harmony.h"
#include "intervals.h"
#include "midi_io.h"
#include "rand.h"

#include "cv_patterns.h"
#include "interval_pattern.h"

static const RandomParam random_503_params[] = {
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
    {OH_DECAY ,  32, 84 },
    {HH_DECAY ,   0, 64 },

    //{MFB_503_CY_LEVEL, 126, 127},
    {MFB_503_CY_MIX  , 100, 127},
    {MFB_503_CY_DECAY, 120, 127},
    {MFB_503_CY_TUNE , 100, 127},

    /*{MFB_503_LT_LEVEL, 126, 127},
    {MFB_503_MT_LEVEL, 126, 127},
    {MFB_503_HT_LEVEL, 126, 127},*/
};
static const uint8_t nr_random_503_params = sizeof(random_503_params) / sizeof(RandomParam);

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
        volume_tom = 0;
        kill_hats = false;
        closed_hat_note = NOTE_503_HH_1;
    }

    void randomize_503_kick()
    {
        // Fill in first or second half of bar
        uint8_t half = Rand::distribution(64, 64);

        uint8_t bar = this->bd_pattern.abPattern.ab_pattern[Rand::randui8(4)];
        this->bd_pattern.patterns[bar].set_kick_fill(half * 8);
    }

    void randomize_503_seq()
    {
        this->bd_pattern.set_coef_kick_pattern();

        this->sd_pattern.set_coef_snare_pattern();

        // Randomize hats
        this->oh_pattern.set_coef_hat_pattern();
        uint8_t four_pat = 0;
        switch (Rand::distribution(10, 10, 10, 10, 10, 10))
        {
        case 0: four_pat = BXXXX; break;
        case 1: four_pat = BXXX0; break;
        case 2: four_pat = BXX0X; break;
        case 3: four_pat = BX0XX; break;
        case 4: four_pat = B0XXX; break;
        case 5: four_pat = BX0X0; break;
        }
        for (int i = 0; i < 3; i++)
        {
            for (int step = 0; step < 4; step++)
            {
                this->hh_pattern.patterns[i].set_gate(step, Utils::gate(four_pat, step));
            }
            this->hh_pattern.length = 4;
            this->hh_pattern.abPattern.set_ab_pattern();
        }
        switch (Rand::distribution(32, 32))
        {
        case 0: this->hat_closed_style = HatClosedStyle::HatClosedRegular; break;
        case 1: this->hat_closed_style = HatClosedStyle::HatClosedInterval; break;
        }
        switch (Rand::distribution(32, 32, 32))
        {
        case 0: this->closed_hat_note = NOTE_503_HH_1; break;
        case 1: this->closed_hat_note = NOTE_503_HH_2; break;
        case 2: this->closed_hat_note = NOTE_503_HH_3; break;
        }
        this->hat_int_pattern.randomize_interval_hat();
        this->hat_velocity.randomize();

        // Randomize Cymbal
        this->cy_pattern.set_coef_kick_pattern();

        // Randomize toms
        this->tom_pattern.randomize();
        this->nr_toms = Rand::randui8(1, 3);
        this->toms_offset = Rand::randui8(3);
        this->tom_mask.randomize_mask_pattern();
    }

    void play_fill(const TimeStruct time)
    {
        if (!Utils::interval_hit(TimeDivision::Sixteenth, time))
            return;

        uint8_t p = 0;
        switch (Rand::randui8(7))
        {
        case 0: p = NOTE_503_BD; break;
        case 1: p = NOTE_503_SD; break;
        case 2: p = NOTE_503_LT; break;
        case 3: p = NOTE_503_MT; break;
        case 4: p = NOTE_503_HT; break;
        case 5: p = NOTE_503_OH; break;
        case 6: p = NOTE_503_HH; break;
        }
        this->storage.note_on(NoteStruct(p, 127));
    }

    void play_roll(const TimeStruct& time)
    {
        static TimeDivision division = TimeDivision::Sixteenth;
        if (Utils::interval_hit(TimeDivision::Sixteenth, time))
        {
            uint8_t r = Rand::randui8(16);
            if (r < 3)
            {
                division = TimeDivision::Thirtysecond;
            }
            else
            {
                division = TimeDivision::Sixteenth;
            }
        }

        if (Utils::interval_hit(division, time))
        {
            this->storage.note_on(NoteStruct(NOTE_503_SD, this->snare_roll));
        }
    }

    void play_bd(HarmonyStruct harmony, const TimeStruct& time)
    {
        if (this->bd_pattern.gate(time) && !this->kill_low)
        {
            uint8_t pitch = NOTE_503_BD;
            if (this->play_pitch_bd)
            {
                pitch = Utils::clip_pitch(harmony.get_chord_step(time), NOTE_503_BD_MIN, NOTE_503_BD_MAX);
            }
            this->storage.note_on(NoteStruct(pitch, 127));
        }
    }

    void play_hats_closed(const TimeStruct& time)
    {
        uint8_t velocity = 63;

        switch (this->hat_closed_style)
        {
        case HatClosedStyle::HatClosedInterval:
            if ((time.tick / TICKS_PER_STEP) % 4 == 0)
                velocity = 127;

            if (this->hat_int_pattern.hit(time))
            {
                this->storage.note_on(NoteStruct(NOTE_503_HH, velocity));
            }
            break;
        case HatClosedStyle::HatClosedRegular:
            if (this->hh_pattern.gate(time))
            {
                velocity = Utils::rerange(this->hat_velocity.value(time), 50, 32);
                this->storage.note_on(NoteStruct(this->closed_hat_note, velocity));
            }
            break;
        }
    }

    void play_hats_open(const TimeStruct& time)
    {
        if (this->kill_hats)
            return;

        uint8_t velocity = 63;
        if ((time.tick / TICKS_PER_STEP) % 4 == 2)
            velocity = 127;

        if (this->oh_pattern.gate(time))
        {
            this->storage.note_on(NoteStruct(NOTE_503_OH, velocity));
        }
    }

    void play_hats(const TimeStruct& time)
    {
        if (this->kill_hats)
            return;

        play_hats_closed(time);
        play_hats_open(time);
    }

    void play_503(HarmonyStruct harmony, const TimeStruct& time)
    {
        if (this->drum_fill)
        {
            return play_fill(time);
        }

        if (this->snare_roll > 0)
        {
            play_roll(time);
        }

        uint8_t velocity = 63;

        // Play kick
        play_bd(harmony, time);

        // Play snare
        if (this->sd_pattern.gate(time) && !this->kill_mid)
        {
            this->storage.note_on(NoteStruct(NOTE_503_SD, velocity));
        }

        // Play hats
        play_hats(time);

        // Play toms
        uint8_t tom_prob = this->tom_pattern.value(time);
        if (Utils::interval_hit(TimeDivision::Sixteenth, time)
            && tom_prob < 100
            && this->tom_mask.gate(time)
            && this->volume_tom > 0)
        {
            uint8_t tom_id = tom_prob % this->nr_toms;
            tom_id = (tom_id + this->toms_offset) % 3;
            uint8_t tom_pitch = NOTE_503_LT;
            if (tom_id == 1)
                tom_pitch = NOTE_503_MT;
            else if (tom_id == 2)
                tom_pitch = NOTE_503_HT;
            this->storage.note_on(NoteStruct(tom_pitch, 64));
        }

        // Play Cymbal
        // TODO: variable velociy
        if (this->cy_pattern.gate(time))
        {
            this->storage.note_on(
                NoteStruct(NOTE_503_CY,
                    64)
            );
        }
    }

    void send_bd_decay()
    {
        uint8_t decay = (uint8_t)((float)this->bd_decay * (.5f + CLIP(this->bd_decay_factor, 0, 127) / 127.f));
        MidiIO::send_cc(BD_DECAY, decay, MIDI_CHANNEL_503);
    }

    void randomize_503_sound()
    {
        for (int i = 0; i < nr_random_503_params; i++)
        {
            MidiIO::send_cc(random_503_params[i].note,
                Rand::randui8(random_503_params[i].min, random_503_params[i].max),
                MIDI_CHANNEL_503);
        }

        // Randomize other sound settings
        this->play_pitch_bd = Rand::randui8(128) < 64;
        this->bd_decay = Rand::randui8(32, 64);
        send_bd_decay();
    }

};
