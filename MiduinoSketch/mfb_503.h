#pragma once

#include "coef.h"
#include "cv.h"
#include "defs.h"
#include "euclid.h"
#include "gate.h"
#include "intervals.h"
#include "midi_io.h"
#include "pitch.h"
#include "rand.h"
#include "rhythms.h"

void randomize_503_seq(ApplicationData& data)
{
    set_euclid(data.settings_503.ac_pattern.pattern, 16, randi(4, 13));
    set_coef_kick_pattern(data.settings_503.bd_pattern);
    set_coef_snare_pattern(data.settings_503.sd_pattern);

    if (randf() < .5)
    {
        set_coef_hat_pattern(data.settings_503.oh_pattern);
        randomize(data.settings_503.hh_pattern, 0.f);
    }
    else
    {
        set_coef_hat_pattern(data.settings_503.hh_pattern);
        randomize(data.settings_503.oh_pattern, 0.f);
    }
    randomize_interval_hat(data.settings_503.hat_int_pattern);

    // Randomize Cymbal
    set_coef_kick_pattern(data.settings_503.cy_pattern);

    // Randomize toms
    randomize(data.settings_503.tom_pattern);
    data.settings_503.nr_toms = randi(1, 3);
    data.settings_503.toms_offset = randi(3);
}

void play_fill(ApplicationData& data)
{
    static TimeDivision division = TimeDivision::TIME_DIVISION_SIXTEENTH;
    if (interval_hit(TimeDivision::TIME_DIVISION_SIXTEENTH, data.step, data.ticks))
    {
        //uint8_t r = randi(16);
        //if (r < 3)
        //{
        //    division = TimeDivision::TIME_DIVISION_THIRTYTWO;
        //}
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
    if (interval_hit(TimeDivision::TIME_DIVISION_SIXTEENTH, data.step, data.ticks))
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
        note_on(NOTE_503_SD, data.uiState.drum_roll, MIDI_CHANNEL_503, data.settings_503.storage);
    }
}

void play_bd(ApplicationData& data)
{
    if (gate(data.settings_503.bd_pattern, data.step, data.ticks) && !data.uiState.kill_low)
    {
        uint8_t pitch = NOTE_503_BD;
        if (data.settings_503.play_pitch_bd)
        {
            pitch = clip_pitch(cv(data.harmony, data.step), NOTE_503_BD_MIN, NOTE_503_BD_MAX);
        }
        note_on(pitch, 127, MIDI_CHANNEL_503, data.settings_503.storage);
    }
}

void play_503(ApplicationData& data)
{
    if (data.uiState.drum_fill)
    {
        return play_fill(data);
    }

    if (data.uiState.drum_roll > 0)
    {
        play_roll(data);
    }

    uint8_t velocity = 63;

    // Play kick
    play_bd(data);

    // Play snare
    if (gate(data.settings_503.sd_pattern, data.step, data.ticks) && !data.uiState.kill_mid)
    {
        note_on(NOTE_503_SD, velocity, MIDI_CHANNEL_503, data.settings_503.storage);
    }

    // Play hats
    switch (data.settings_503.hat_style)
    {
    case HatStyle::HatOffBeat:
    {
        bool hh = gate(data.settings_503.hh_pattern, data.step, data.ticks);
        if (gate(data.settings_503.oh_pattern, data.step, data.ticks) && !hh && !data.uiState.kill_high)
        {
            note_on(NOTE_503_OH, velocity, MIDI_CHANNEL_503, data.settings_503.storage);
        }
        if (hh && !data.uiState.kill_high)
        {
            note_on(NOTE_503_HH, velocity, MIDI_CHANNEL_503, data.settings_503.storage);
        }
        break;
    }
    case HatStyle::HatFull:
        if (interval_hit(data.settings_503.hat_int_pattern, data.step, data.ticks) && !data.uiState.kill_high)
        {
            note_on(NOTE_503_OH, velocity, MIDI_CHANNEL_503, data.settings_503.storage);
        }
        break;
    }

    // Play toms
    if (interval_hit(TimeDivision::TIME_DIVISION_SIXTEENTH, data.step, data.ticks) 
        && data.settings_503.velocity_tom > 0)
    {
        uint8_t tom_id = cv(data.settings_503.tom_pattern, data.step) % data.settings_503.nr_toms;
        tom_id = (tom_id + data.settings_503.toms_offset) % 3;
        uint8_t tom_pitch = NOTE_503_LT;
        if (tom_id == 1)
            tom_pitch = NOTE_503_MT;
        else if (tom_id == 2)
            tom_pitch = NOTE_503_HT;
        note_on(tom_pitch, data.settings_503.velocity_tom, MIDI_CHANNEL_503, data.settings_503.storage);
    }

    // Play Cymbal
    if (data.settings_503.velocity_cy > 0)
    {
        if (gate(data.settings_503.cy_pattern, data.step, data.ticks))
        {
            note_on(NOTE_503_CY, 
                    data.settings_503.velocity_cy, 
                    MIDI_CHANNEL_503, 
                    data.settings_503.storage);
        }
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

    {MFB_503_CY_LEVEL, 126, 127},
    {MFB_503_CY_MIX  , 100, 127},
    {MFB_503_CY_DECAY, 126, 127},
    {MFB_503_CY_TUNE ,   0, 127},

    {MFB_503_LT_LEVEL, 126, 127},
    {MFB_503_MT_LEVEL, 126, 127},
    {MFB_503_HT_LEVEL, 126, 127},
};
const uint8_t nr_random_503_params = sizeof(random_503_params) / sizeof(RandomParam);

void send_bd_decay(ApplicationData& data)
{
    uint8_t decay = (uint8_t)((float)data.settings_503.bd_decay * (.5f + CLIP(data.uiState.bd_decay_factor, 0, 127) / 127.f));
    send_cc(BD_DECAY, decay, MIDI_CHANNEL_503);
}

void randomize_503_sound(ApplicationData& data)
{
    for (int i = 0; i < nr_random_503_params; i++)
    {
        send_cc(random_503_params[i].note, 
                randi(random_503_params[i].min, random_503_params[i].max), 
                MIDI_CHANNEL_503);
    }

    // Randomize Tom sounds
    uint8_t tom_dec = randi(TOM_DEC_MIN, TOM_DEC_MAX);
    send_cc(MFB_503_LT_DECAY, tom_dec, MIDI_CHANNEL_503);
    send_cc(MFB_503_MT_DECAY, tom_dec, MIDI_CHANNEL_503);
    send_cc(MFB_503_HT_DECAY, tom_dec, MIDI_CHANNEL_503);

    uint8_t tom_tune = randi(16, 32);
    uint8_t tom_spread = randi(12, 24);
    send_cc(MFB_503_LT_TUNE, tom_tune, MIDI_CHANNEL_503);
    send_cc(MFB_503_MT_TUNE, tom_tune + tom_spread, MIDI_CHANNEL_503);
    send_cc(MFB_503_HT_TUNE, tom_tune + tom_spread * 2, MIDI_CHANNEL_503);

    uint8_t tom_pitch = randi(16, 32);
    send_cc(MFB_503_LT_PITCH, tom_pitch, MIDI_CHANNEL_503);
    send_cc(MFB_503_MT_PITCH, tom_pitch + tom_spread, MIDI_CHANNEL_503);
    send_cc(MFB_503_HT_PITCH, tom_pitch + tom_spread * 2, MIDI_CHANNEL_503);

    send_cc(MFB_503_LT_LEVEL, 127, MIDI_CHANNEL_503);
    send_cc(MFB_503_MT_LEVEL, 127, MIDI_CHANNEL_503);
    send_cc(MFB_503_HT_LEVEL, 127, MIDI_CHANNEL_503);

    // Randomize other sound settings
    data.settings_503.play_pitch_bd = randi(128) < 32;
    data.settings_503.bd_decay = randi(32, 64);
    send_bd_decay(data);
}
