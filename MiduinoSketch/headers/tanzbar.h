#pragma once

#include "mfb_503.h"

#include "ab.h"
#include "coef.h"
#include "cv.h"
#include "defs.h"
#include "euclid.h"
#include "gate.h"
#include "harmony.h"
#include "intervals.h"
#include "mask.h"
#include "midi_io.h"
#include "pitch.h"
#include "rand.h"
#include "rhythms.h"

void randomize_tanzbar_kick(TanzbarSettings& settings)
{
    // Fill in first or second half of bar
    uint8_t half = distribution(64, 64);

    uint8_t bar = settings.bd_pattern.abPattern[randi(4)];
    set_kick_fill(settings.bd_pattern.patterns[bar], half * 8);
}

void randomize_tanzbar(TanzbarSettings& settings)
{
    set_coef_kick_pattern(settings.bd_pattern);

    set_coef_snare_pattern(settings.sd_pattern);

    // Randomize hats
    set_coef_hat_pattern(settings.oh_pattern);
    uint8_t four_pat = 0;
    switch (distribution(10, 10, 10, 10, 10, 10))
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
            set_gate(settings.hh_pattern.patterns[i], step, gate(four_pat, step));
        }
        settings.hh_pattern.length = 4;
        set_ab_pattern(settings.hh_pattern.abPattern);
    }
    switch (distribution(32, 32))
    {
    case 0: settings.hat_closed_style = HatClosedStyle::HatClosedRegular; break;
    case 1: settings.hat_closed_style = HatClosedStyle::HatClosedInterval; break;
    }
    switch (distribution(32, 32, 32))
    {
    case 0: settings.closed_hat_note = NOTE_503_HH_1; break;
    case 1: settings.closed_hat_note = NOTE_503_HH_2; break;
    case 2: settings.closed_hat_note = NOTE_503_HH_3; break;
    }
    randomize_interval_hat(settings.hat_int_pattern);
    randomize(settings.hat_velocity);

    // Randomize Cymbal
    set_coef_kick_pattern(settings.cy_pattern);

    // Randomize toms
    randomize(settings.tom_pattern);
    settings.nr_toms = randi(1, 3);
    settings.toms_offset = randi(3);
    randomize_mask_pattern(settings.tom_mask);
}

void play_fill(TanzbarSettings& settings, const TimeStruct time)
{
    if (!interval_hit(TimeDivision::Sixteenth, time))
        return;

    uint8_t p = 0;
    switch (randi(7))
    {
    case 0: p = NOTE_TANZBAR_BD1; break;
    case 1: p = NOTE_TANZBAR_SD; break;
    case 2: p = NOTE_TANZBAR_LT; break;
    case 3: p = NOTE_TANZBAR_MT; break;
    case 4: p = NOTE_TANZBAR_HT; break;
    case 5: p = NOTE_TANZBAR_OH; break;
    case 6: p = NOTE_TANZBAR_HH; break;
    }
    note_on(make_note(p, 127), settings.storage);
}

void play_roll(TanzbarSettings& settings, const TimeStruct& time)
{
    static TimeDivision division = TimeDivision::Sixteenth;
    if (interval_hit(TimeDivision::Sixteenth, time))
    {
        uint8_t r = randi(16);
        if (r < 3)
        {
            division = TimeDivision::Thirtysecond;
        }
        else
        {
            division = TimeDivision::Sixteenth;
        }
    }

    if (interval_hit(division, time))
    {
        note_on(make_note(NOTE_503_SD, settings.snare_roll), settings.storage);
    }
}

void play_bd(TanzbarSettings& settings, const TimeStruct& time)
{
    if (gate(settings.bd_pattern, time) && !settings.kill_low)
    {
        uint8_t pitch = NOTE_503_BD;
        note_on(make_note(pitch, 127), settings.storage);
    }
}

void play_hats_closed(TanzbarSettings& settings, const TimeStruct& time)
{
    uint8_t velocity = 63;

    switch (settings.hat_closed_style)
    {
    case HatClosedStyle::HatClosedInterval:
        if (time.step % 4 == 0)
            velocity = 127;

        if (interval_hit(settings.hat_int_pattern, time))
        {
            note_on(make_note(NOTE_503_HH, velocity), settings.storage);
        }
        break;
    case HatClosedStyle::HatClosedRegular:
        if (gate(settings.hh_pattern, time))
        {
            velocity = apply_cv(cv(settings.hat_velocity, time.step), 50, 32);
            note_on(make_note(settings.closed_hat_note, velocity), settings.storage);
        }
        break;
    }
}

void play_hats_open(TanzbarSettings& settings, const TimeStruct& time)
{
    if (settings.kill_hats)
        return;

    uint8_t velocity = 63;
    if (time.step % 4 == 2)
        velocity = 127;

    if (gate(settings.oh_pattern, time))
    {
        note_on(make_note(NOTE_503_OH, velocity), settings.storage);
    }
}

void play_hats(TanzbarSettings& settings, const TimeStruct& time)
{
    if (settings.kill_hats)
        return;

    switch (settings.hat_style)
    {
    case HatStyle::HatOpen:
        play_hats_open(settings, time);
        break;
    case HatStyle::HatBoth:
        play_hats_closed(settings, time);
        play_hats_open(settings, time);
        break;
    case HatStyle::HatClosed:
        play_hats_closed(settings, time);
        break;
    }
}

void play_tanzbar(TanzbarSettings& settings, const TimeStruct& time)
{
    if (settings.drum_fill)
    {
        return play_fill(settings, time);
    }

    if (settings.snare_roll > 0)
    {
        play_roll(settings, time);
    }

    uint8_t velocity = 63;

    // Play kick
    play_bd(settings, time);

    // Play snare
    if (gate(settings.sd_pattern, time) && !settings.kill_mid)
    {
        note_on(make_note(NOTE_503_SD, velocity), settings.storage);
    }

    // Play hats
    play_hats(settings, time);

    // Play toms
    uint8_t tom_prob = cv(settings.tom_pattern, time.step);
    if (interval_hit(TimeDivision::Sixteenth, time) 
        && tom_prob < 100
        && gate(settings.tom_mask, time)
        && settings.volume_tom > 0)
    {
        uint8_t tom_id = tom_prob % settings.nr_toms;
        tom_id = (tom_id + settings.toms_offset) % 3;
        uint8_t tom_pitch = NOTE_503_LT;
        if (tom_id == 1)
            tom_pitch = NOTE_503_MT;
        else if (tom_id == 2)
            tom_pitch = NOTE_503_HT;
        note_on(make_note(tom_pitch, 64), settings.storage);
    }

    // Play Cymbal
    if (settings.volume_cy > 0)
    {
        if (gate(settings.cy_pattern, time))
        {
            note_on(make_note(NOTE_503_CY,
                    settings.volume_cy), 
                    settings.storage);
        }
    }
}

const RandomParam random_tanzbar_params[] = {
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
};
const uint8_t nr_random_tanzbar_params = sizeof(random_tanzbar_params) / sizeof(RandomParam);

void randomize_tanzbar_sound(TanzbarSettings& settings)
{
    for (int i = 0; i < nr_random_tanzbar_params; i++)
    {
        send_cc(random_tanzbar_params[i].note, 
                randi(random_tanzbar_params[i].min, random_tanzbar_params[i].max), 
                MIDI_CHANNEL_TANZBAR);
    }

    // Randomize other sound settings
    settings.bd_decay = randi(32, 64);
}
