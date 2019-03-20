#pragma once

#include "defs.h"
#include "modulators.h"
#include "mfb_503.h"
#include "timing_structs.h"

#define MODULATOR_PROB (.2f)

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

class TanzbarModulators
{
public:
    ModulationReceiver cl_pitch;
    ModulationReceiver ma_dec;
    ModulationReceiver rs_tune;
    ModulationReceiver cb_tune;
    ModulationReceiver cp_trig;
    ModulationReceiver cy_tune;

    void randomize()
    {
        uint8_t range = randui8(128, 64);
        this->cl_pitch.randomize(range, 127 - range, MODULATOR_PROB);

        this->ma_dec.randomize(randui8(128, 64), 0);

        range = randui8(128);
        this->rs_tune.randomize(range, 127 - range, MODULATOR_PROB);

        range = randui8(128);
        this->cb_tune.randomize(range, 127 - range, MODULATOR_PROB);

        range = randui8(128);
        this->cp_trig.randomize(range, 127 - range, MODULATOR_PROB);

        range = randui8(128);
        this->cy_tune.randomize(range, 127 - range, MODULATOR_PROB);
    }
};

class TanzbarSettings 
{
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

    TanzbarModulators modulators;
    TanzbarTimeSettings time_settings;

    PitchStorage storage;

    TanzbarSettings()
    {
        hat_closed_style = HatClosedStyle::HatClosedRegular;
        kill_hats = false;
        percussion_type = PercussionType::PercussionToms;
    }

    void randomize_tanzbar_kick()
    {
        // Fill in first or second half of bar
        uint8_t half = distribution(64, 64);

        uint8_t bar = this->bd_pattern.abPattern[randui8(4)];
        set_kick_fill(this->bd_pattern.patterns[bar], half * 8);
    }

    void randomize_timing()
    {
        this->time_settings.bd.randomize();
        this->time_settings.cb.randomize();
        this->time_settings.cl.randomize();
        this->time_settings.cp.randomize();
        this->time_settings.cy.randomize();
        this->time_settings.hh.randomize();
        this->time_settings.ma.randomize();
        this->time_settings.sd.randomize();
        this->time_settings.tc.randomize();
    }

    void randomize_tanzbar()
    {
        set_coef_kick_pattern(this->bd_pattern);

        set_coef_snare_pattern(this->sd_pattern);
        set_coef_snare_pattern(this->cp_pattern);

        this->rs_pattern.randomize();

        // Randomize hats
        set_coef_hat_pattern(this->oh_pattern);
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
                this->hh_pattern.patterns[i].set_gate(step, gate(four_pat, step));
            }
            this->hh_pattern.length = 4;
            set_ab_pattern(this->hh_pattern.abPattern);
        }
        switch (distribution(32, 32))
        {
        case 0: this->hat_closed_style = HatClosedStyle::HatClosedRegular; break;
        case 1: this->hat_closed_style = HatClosedStyle::HatClosedInterval; break;
        }

        randomize_interval_hat(this->hat_int_pattern);
        this->hat_velocity.randomize();

        // Randomize Maracas
        this->ma_pattern.randomize();
        this->ma_pattern.time_division = TimeDivision::Sixteenth;
        switch (distribution(32, 32, 32))
        {
        case 0: this->ma_pattern.length = 2; break;
        case 1: this->ma_pattern.length = 4; break;
        case 2: this->ma_pattern.length = 8; break;
        }

        // Randomize Cymbal
        switch (distribution(16, 16, 16))
        {
        case 0:
            set_coef_kick_pattern(this->cy_pattern);
            this->cy_pattern.length = 16;
            break;
        case 1:
            set_euclid(this->cy_pattern, 8, 3);
            this->cy_pattern.length = 8;
            break;
        case 2:
            set_coef_hat_pattern(this->cy_pattern);
            this->cy_pattern.length = 16;
            break;
        }

        // Randomize toms
        this->tom_pattern.randomize();
        this->toms_offset = randui8(3);
        switch (distribution(32, 32))
        {
        case 0: this->percussion_type = PercussionType::PercussionToms; break;
        case 1: this->percussion_type = PercussionType::PercussionCongas; break;
        }
        this->tom_mask.randomize_mask_pattern();

        // Randomize other percussion
        this->cl_pattern.randomize();
        this->cb_pattern.randomize();

        // Randomize micro-timing
        this->randomize_timing();

        // Randomize modulators
        this->modulators.randomize();
    }

    void play_fill(const TimeStruct time)
    {
        if (!interval_hit(TimeDivision::Sixteenth, time))
            return;

        uint8_t p = 0;
        switch (randui8(7))
        {
        case 0: p = NOTE_TANZBAR_BD1; break;
        case 1: p = NOTE_TANZBAR_SD; break;
        case 2: p = NOTE_TANZBAR_LT; break;
        case 3: p = NOTE_TANZBAR_MT; break;
        case 4: p = NOTE_TANZBAR_HT; break;
        case 5: p = NOTE_TANZBAR_OH; break;
        case 6: p = NOTE_TANZBAR_HH; break;
        }
        this->storage.note_on(make_note(p, 127), time.get_shuffle_delay());
    }

    void play_roll(const TimeStruct& time)
    {
        static TimeDivision division = TimeDivision::Sixteenth;
        if (interval_hit(TimeDivision::Sixteenth, time))
        {
            uint8_t r = randui8(16);
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
            this->storage.note_on(
                make_note(NOTE_TANZBAR_SD, this->snare_roll),
                time.get_shuffle_delay(this->time_settings.sd)
            );
        }
    }

    void play_bd(const TimeStruct& time)
    {
        bool quarter_hit = interval_hit(TimeDivision::Quarter, time);
        uint8_t velocity = quarter_hit ? 127 : 63;
        if (this->bd_pattern.gate(time) && !this->kill_low)
        {
            uint8_t pitch = NOTE_TANZBAR_BD1;
            this->storage.note_on(
                make_note(pitch, velocity),
                time.get_shuffle_delay(this->time_settings.bd)
            );
        }

        if (quarter_hit && !this->kill_low)
        {
            this->storage.note_on(
                make_note(NOTE_TANZBAR_BD2, velocity),
                time.get_shuffle_delay(this->time_settings.bd)
            );
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

            if (interval_hit(this->hat_int_pattern, time))
            {
                this->storage.note_on(
                    make_note(NOTE_TANZBAR_HH, velocity),
                    time.get_shuffle_delay(this->time_settings.hh)
                );
            }
            break;
        case HatClosedStyle::HatClosedRegular:
            if (this->hh_pattern.gate(time))
            {
                velocity = rerange(this->hat_velocity.value(time), 50, 32);
                this->storage.note_on(
                    make_note(NOTE_TANZBAR_HH, velocity),
                    time.get_shuffle_delay(this->time_settings.hh)
                );
            }
            break;
        }
    }

    bool play_hats_open(const TimeStruct& time)
    {
        if (this->kill_hats)
            return false;

        uint8_t velocity = 63;
        if ((time.tick / TICKS_PER_STEP) % 4 == 2)
            velocity = 127;

        if (this->oh_pattern.gate(time))
        {
            this->storage.note_on(
                make_note(NOTE_TANZBAR_OH, velocity),
                time.get_shuffle_delay(this->time_settings.hh)
            );
            return true;
        }
        return false;
    }

    void play_hats(const TimeStruct& time)
    {
        if (this->kill_hats)
            return;

        if (!play_hats_open(time))
        {
            play_hats_closed(time);
        }
    }

    bool play_maracas(
        const Modulators& modulators,
        const TimeStruct& time)
    {
        if (interval_hit(this->ma_pattern.time_division, time))
        {
            send_cc(
                TB_MA_Decay,
                this->modulators.ma_dec.value(modulators, time),
                MIDI_CC_CHANNEL_TANZBAR
            );

            this->storage.note_on(
                make_note(NOTE_TANZBAR_MA, rerange(this->ma_pattern.value(time), 96, 16)),
                time.get_shuffle_delay(this->time_settings.ma)
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

    void play(
        const Modulators& modulators,
        const TimeStruct& time)
    {
        if (this->drum_fill)
        {
            return this->play_fill(time);
        }

        if (this->snare_roll > 0)
        {
            this->play_roll(time);
        }

        uint8_t velocity = 63;

        // Play kick
        this->play_bd(time);

        // Play snare
        if (this->sd_pattern.gate(time) && !this->kill_mid)
        {
            this->storage.note_on(
                make_note(NOTE_TANZBAR_SD, velocity),
                time.get_shuffle_delay(this->time_settings.sd)
            );
        }

        // Play rimshot
        if (this->rs_pattern.gate(time) && !this->kill_mid)
        {
            uint8_t value = 0;
            if (this->modulators.rs_tune.value(modulators, time))
            {
                send_cc(TB_RS_TUNE, value, MIDI_CC_CHANNEL_TANZBAR);
            }
            this->storage.note_on(
                make_note(NOTE_TANZBAR_RS, velocity),
                time.get_shuffle_delay(this->time_settings.sd)
            );
        }

        // Play clap
        if (this->cp_pattern.gate(time) && !this->kill_mid)
        {
            uint8_t value = 0;
            if (this->modulators.cp_trig.value(modulators, time))
            {
                send_cc(TB_CP_TRIGGER, value, MIDI_CC_CHANNEL_TANZBAR);
            }
            this->storage.note_on(
                make_note(NOTE_TANZBAR_CP, velocity),
                time.get_shuffle_delay(this->time_settings.cp)
            );
        }

        // Play clave
        if (this->cl_pattern.gate(time))
        {
            uint8_t value = 0;
            if (this->modulators.cl_pitch.value(modulators, time, value))
            {
                send_cc(TB_CL_TUNE, value, MIDI_CC_CHANNEL_TANZBAR);
            }
            this->storage.note_on(
                make_note(NOTE_TANZBAR_CL, velocity),
                time.get_shuffle_delay(this->time_settings.cl)
            );
        }

        // Play cowbell
        if (this->cb_pattern.gate(time))
        {
            uint8_t value = 0;
            if (this->modulators.cb_tune.value(modulators, time))
            {
                send_cc(TB_CB_Tune, value, MIDI_CC_CHANNEL_TANZBAR);
            }
            this->storage.note_on(
                make_note(NOTE_TANZBAR_CB, velocity),
                time.get_shuffle_delay(this->time_settings.cb)
            );
        }

        // Play hats
        this->play_hats(time);

        this->play_maracas(modulators, time);

        // Play toms
        uint8_t tom_prob = this->tom_pattern.value(time);
        if (interval_hit(TimeDivision::Sixteenth, time)
            && tom_prob < 100
            && this->tom_mask.gate(time))
        {
            uint8_t tom_id = tom_prob % 3;
            tom_id = (tom_id + this->toms_offset) % 3;
            this->storage.note_on(
                make_note(get_tc_pitch(tom_id, this->percussion_type), 64),
                time.get_shuffle_delay(this->time_settings.tc)
            );
        }

        // Play Cymbal
        if (this->cy_pattern.gate(time))
        {
            uint8_t value = 0;
            if (this->modulators.cy_tune.value(modulators, time))
            {
                send_cc(TB_CY_TUNE, value, MIDI_CC_CHANNEL_TANZBAR);
            }
            this->storage.note_on(
                make_note(NOTE_TANZBAR_CY, 64),
                time.get_shuffle_delay(this->time_settings.cy)
            );
        }
    }

    void randomize_tanzbar_sound()
    {
        for (int i = 0; i < nr_random_tanzbar_params; i++)
        {
            send_cc(random_tanzbar_params[i].note,
                randui8(random_tanzbar_params[i].min, random_tanzbar_params[i].max),
                MIDI_CC_CHANNEL_TANZBAR);
        }
    }

};

