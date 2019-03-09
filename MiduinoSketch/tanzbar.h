#pragma once

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

void randomize_timing(MicroTimingStruct& settings)
{
    settings.delay = randi();
    settings.shuffle_off = randi8(-20, 20);
}

void randomize_timing(TanzbarTimeSettings& settings)
{
    randomize_timing(settings.bd);
    randomize_timing(settings.cb);
    randomize_timing(settings.cl);
    randomize_timing(settings.cp);
    randomize_timing(settings.cy);
    randomize_timing(settings.hh);
    randomize_timing(settings.ma);
    randomize_timing(settings.sd);
    randomize_timing(settings.tc);
}

void randomize_tanzbar(TanzbarSettings& settings)
{
    set_coef_kick_pattern(settings.bd_pattern);

    set_coef_snare_pattern(settings.sd_pattern);
    set_coef_snare_pattern(settings.cp_pattern);

    randomize(settings.rs_pattern);

    // Randomize hats
    set_coef_hat_pattern(settings.oh_pattern);
    uint8_t four_pat = 0;
    switch (distribution(32, 10, 10, 10, 10))
    {
    case 0: four_pat = BXXXX; break;
    case 1: four_pat = BXXX0; break;
    case 2: four_pat = BXX0X; break;
    case 3: four_pat = BX0XX; break;
    case 4: four_pat = B0XXX; break;
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

    randomize_interval_hat(settings.hat_int_pattern);
    randomize(settings.hat_velocity);

    // Randomize Maracas
    randomize(settings.ma_pattern);
    settings.ma_pattern.time_division = TimeDivision::Sixteenth; 
    switch (distribution(32, 32, 32))
    {
    case 0: settings.ma_pattern.length = 2; break;
    case 1: settings.ma_pattern.length = 4; break;
    case 2: settings.ma_pattern.length = 8; break;
    }
    settings.modulate_ma_offset = randi(0, 64);
    settings.modulate_ma_range = randi(16, 64);

    // Randomize Cymbal
    switch (distribution(16, 16, 16))
    {
    case 0: 
        set_coef_kick_pattern(settings.cy_pattern);
        settings.cy_pattern.length = 16;
        break;
    case 1:
        set_euclid(settings.cy_pattern, 8, 3);
        settings.cy_pattern.length = 8;
        break;
    case 2:
        set_coef_hat_pattern(settings.cy_pattern);
        settings.cy_pattern.length = 16;
        break;
    }

    // Randomize toms
    randomize(settings.tom_pattern);
    settings.toms_offset = randi(3);
    switch (distribution(32, 32))
    {
        case 0: settings.percussion_type = PercussionType::PercussionToms; break;
        case 1: settings.percussion_type = PercussionType::PercussionCongas; break;
    }
    randomize_mask_pattern(settings.tom_mask);

    // Randomize other percussion
    randomize(settings.cl_pattern);
    randomize(settings.cb_pattern);

    // Randomize micro-timing
    randomize_timing(settings.time_settings);
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
    note_on(make_note(p, 127), settings.storage, get_shuffle_delay(time));
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
        note_on(make_note(NOTE_TANZBAR_SD, settings.snare_roll), settings.storage, get_shuffle_delay(time, settings.time_settings.sd));
    }
}

void play_bd(TanzbarSettings& settings, const TimeStruct& time)
{
    bool quarter_hit = interval_hit(TimeDivision::Quarter, time);
    uint8_t velocity = quarter_hit ? 127 : 63;
    if (gate(settings.bd_pattern, time) && !settings.kill_low)
    {
        uint8_t pitch = NOTE_TANZBAR_BD1;
        note_on(make_note(pitch, velocity), settings.storage, get_shuffle_delay(time, settings.time_settings.bd));
    }

    if (quarter_hit && !settings.kill_low)
    {
        note_on(make_note(NOTE_TANZBAR_BD2, velocity), settings.storage, get_shuffle_delay(time, settings.time_settings.bd));
    }
}

void play_hats_closed(TanzbarSettings& settings, const TimeStruct& time)
{
    uint8_t velocity = 63;

    switch (settings.hat_closed_style)
    {
    case HatClosedStyle::HatClosedInterval:
        if ((time.tick / TICKS_PER_STEP) % 4 == 0)
            velocity = 127;

        if (interval_hit(settings.hat_int_pattern, time))
        {
            note_on(
                make_note(NOTE_TANZBAR_HH, velocity), 
                settings.storage, 
                get_shuffle_delay(time, settings.time_settings.hh)
            );
        }
        break;
    case HatClosedStyle::HatClosedRegular:
        if (gate(settings.hh_pattern, time))
        {
            velocity = apply_cv(cv(settings.hat_velocity, time), 50, 32);
            note_on(
                make_note(NOTE_TANZBAR_HH, velocity), 
                settings.storage, 
                get_shuffle_delay(time, settings.time_settings.hh)
            );
        }
        break;
    }
}

bool play_hats_open(TanzbarSettings& settings, const TimeStruct& time)
{
    if (settings.kill_hats)
        return false;

    uint8_t velocity = 63;
    if ((time.tick / TICKS_PER_STEP) % 4 == 2)
        velocity = 127;

    if (gate(settings.oh_pattern, time))
    {
        note_on(
            make_note(NOTE_TANZBAR_OH, velocity), 
            settings.storage,
            get_shuffle_delay(time, settings.time_settings.hh)
        );
        return true;
    }
    return false;
}

void play_hats(TanzbarSettings& settings, const TimeStruct& time)
{
    if (settings.kill_hats)
        return;

    if (!play_hats_open(settings, time))
    {
        play_hats_closed(settings, time);
    }
}

bool play_maracas(TanzbarSettings& settings, const TimeStruct& time)
{
    if (interval_hit(settings.ma_pattern.time_division, time))
    {
        send_cc(
            TB_MA_Decay, 
            quad(apply_cv(
                cv(settings.ma_pattern, time), 
                settings.modulate_ma_range, 
                settings.modulate_ma_offset
            )),
            MIDI_CC_CHANNEL_TANZBAR
        );
        note_on(
            make_note(NOTE_TANZBAR_MA, apply_cv(cv(settings.ma_pattern, time), 96, 16)), 
            settings.storage,
            get_shuffle_delay(time, settings.time_settings.ma)
        );
        return true;
    }
    return false;
}

uint8_t get_tc_pitch(uint8_t id, PercussionType type)
{
    static const uint8_t t_pitches[3] = { NOTE_TANZBAR_LT, NOTE_TANZBAR_MT, NOTE_TANZBAR_HT };
    static const uint8_t c_pitches[3] = { NOTE_TANZBAR_LC, NOTE_TANZBAR_MC, NOTE_TANZBAR_HC };
    switch (type)
    {
    case PercussionType::PercussionToms:
        return t_pitches[id % 3];
    case PercussionType::PercussionCongas:
    default:
        return c_pitches[id % 3];
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
        note_on(
            make_note(NOTE_TANZBAR_SD, velocity), 
            settings.storage, 
            get_shuffle_delay(time, settings.time_settings.sd)
        );
    }

    // Play rimshot
    if (gate(settings.rs_pattern, time) && !settings.kill_mid)
    {
        note_on(
            make_note(NOTE_TANZBAR_RS, velocity), 
            settings.storage, 
            get_shuffle_delay(time, settings.time_settings.sd)
        );
    }

    // Play clap
    if (gate(settings.cp_pattern, time) && !settings.kill_mid)
    {
        note_on(make_note(NOTE_TANZBAR_CP, velocity), settings.storage, get_shuffle_delay(time, settings.time_settings.cp));
    }

    // Play clave
    if (gate(settings.cl_pattern, time))
    {
        note_on(
            make_note(NOTE_TANZBAR_CL, velocity),
            settings.storage,
            get_shuffle_delay(time, settings.time_settings.cl)
        );
    }

    // Play clave
    if (gate(settings.cb_pattern, time))
    {
        note_on(
            make_note(NOTE_TANZBAR_CB, velocity),
            settings.storage,
            get_shuffle_delay(time, settings.time_settings.cb)
        );
    }

    // Play hats
    play_hats(settings, time);

    play_maracas(settings, time);

    // Play toms
    uint8_t tom_prob = cv(settings.tom_pattern, time);
    if (interval_hit(TimeDivision::Sixteenth, time) 
        && tom_prob < 100
        && gate(settings.tom_mask, time))
    {
        uint8_t tom_id = tom_prob % 3;
        tom_id = (tom_id + settings.toms_offset) % 3;
        note_on(
            make_note(get_tc_pitch(tom_id, settings.percussion_type), 64),
            settings.storage, 
            get_shuffle_delay(time, settings.time_settings.tc)
        );
    }

    // Play Cymbal
    if (gate(settings.cy_pattern, time))
    {
        note_on(
            make_note(NOTE_TANZBAR_CY, 64), 
            settings.storage, 
            get_shuffle_delay(time, settings.time_settings.cy)
        );
    }
}

const RandomParam random_tanzbar_params[] = {
    {TB_BD1_ATTACK   ,  0, 127},
    {TB_BD1_DECAY    , 32,  96},
    {TB_BD1_PITCH    , 32,  96},
    {TB_BD1_TUNE     , 32,  96},
    {TB_BD1_NOISE    ,  0, 127},
    {TB_BD1_FILTER   ,  0, 127},
    {TB_BD1_DIST     ,  0, 127},
    {TB_BD1_TRIGGER  ,  0, 127},
                     
    {TB_BD2_DECAY    , 32,  96},
    {TB_BD2_TUNE     , 32,  96},
    {TB_BD2_TONE     , 32,  96},

    {TB_SD_TUNE      ,  0, 127},
    {TB_SD_DTUNE     ,  0, 127},
    {TB_SD_SNAPPY    , 64, 127},
    {TB_SD_SN_DECAY  , 64, 127},
    {TB_SD_TONE      ,  0, 127},
    {TB_SD_TONE_DECAY,  0,  64},
    {TB_SD_PITCH     ,  0, 127},

    {TB_RS_TUNE      ,  0, 127},

    {TB_OH_DECAY,  0,  96},
    {TB_HH_TUNE ,  0, 127},
    {TB_HH_DECAY,  0, 127},

    {TB_CY_DECAY,  64, 127},
    {TB_CY_TONE ,   0, 127},
    {TB_CY_TUNE ,   0, 127},

    {TB_CL_TUNE           , 32,  96},
    {TB_CL_DECAY          , 32,  96},
    {TB_CP_DECAY          , 32,  96},
    {TB_CP_FILTER         , 32,  96},
    {TB_CP_ATTACK         , 32,  96},
    {TB_CP_TRIGGER        , 32,  96},
    {TB_HTC_TUNE          , 32,  96},
    {TB_HTC_DECAY         , 32,  96},
    {TB_HTC_NOISE_ON_OFF  , 32,  96},
    {TB_HTC_TOM_CONGA     , 32,  96},
    {TB_MTC_TUNE          , 32,  96},
    {TB_MTC_DECAY         , 32,  96},
    {TB_MTC_NOISE_ON_OFF  , 32,  96},
    {TB_MTC_TOM_CONGA     , 32,  96},
    {TB_LTC_TUNE          , 32,  96},
    {TB_LTC_DECAY         , 32,  96},
    {TB_LTC_NOISE_ON_OFF  , 32,  96},
    {TB_LTC_TOM_CONGA     , 32,  96},
    {TB_TOM_NOISE         , 32,  96},
    {TB_CB_Tune           , 32,  96},
    {TB_CB_Decay          , 32,  96},
    {TB_MA_Decay          , 32,  96},
};
const uint8_t nr_random_tanzbar_params = sizeof(random_tanzbar_params) / sizeof(RandomParam);

void randomize_tanzbar_sound(TanzbarSettings& settings)
{
    for (int i = 0; i < nr_random_tanzbar_params; i++)
    {
        send_cc(random_tanzbar_params[i].note, 
                randi(random_tanzbar_params[i].min, random_tanzbar_params[i].max), 
                MIDI_CC_CHANNEL_TANZBAR);
    }
}
